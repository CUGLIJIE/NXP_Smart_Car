
//根据超核库2.3版本修改而来，可以实现参数掉电保存

#include "Flash_Date.h"
#include "MK60D10.h"
#include "stdint.h"
#include "MK60_uart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"	 
/***********************************************************************************************
 功能：内部函数 检查命令是否完成
 形参：0
 返回：FLASH_OK 成功    FLASH_ERROR 失败 
 详解：
************************************************************************************************/
static uint8_t CommandLaunch(void)
{
    // 清除访问错误标志位和非法访问标志位 
    FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    // 启动命令
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    // 等待命令结束
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0);
    // 检查错误标志
    if(FTFL->FSTAT & (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_MGSTAT0_MASK|FTFL_FSTAT_RDCOLERR_MASK))
    return(FLASH_ERROR); //出错
    return (FLASH_OK); //成功
}

/***********************************************************************************************
 功能：初始化内部Flash模块
 形参：0
 返回：0
 详解：
************************************************************************************************/
void FLASH_Init(void)
{
	//清除FMC缓冲区
	FMC->PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;
	FMC->PFB1CR |= FMC_PFB0CR_S_B_INV_MASK;
	// 禁止看门狗
	WDOG->UNLOCK = 0xC520;
	WDOG->UNLOCK = 0xD928;
	WDOG->STCTRLH = 0;    // 禁止看门狗
	//检查Flash访问错误
  if(FTFL->FSTAT & FTFL_FSTAT_ACCERR_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_ACCERR_MASK;       //清除错误标志
  }
  //检查保护错误
  else if (FTFL->FSTAT & FTFL_FSTAT_FPVIOL_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_FPVIOL_MASK;
  }
  //检查读冲突错误
  else if (FTFL->FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;
  }
  //禁用Flash模块的数据缓存
  FMC->PFB0CR &= ~FMC_PFB0CR_B0DCE_MASK;
  FMC->PFB1CR &= ~FMC_PFB1CR_B1DCE_MASK;
}
/***********************************************************************************************
 功能：读取Flash数据
 形参：start_addr: 其实地址  
             len: 要读取的长度
		 uint8_t *pbuffer: 参数指针
 返回：0
 详解：
************************************************************************************************/
void FLASH_Read(uint32_t FlashStartAdd,uint32_t len,uint8_t *pbuffer)
{
	uint32_t i=0;
	for(i=0;i<len;i++)
	{
	  pbuffer[i]=*(uint8_t *)(FlashStartAdd+i);		//读取指定地址的数据
	}
}

/***********************************************************************************************
 功能：写一个扇区(一个扇区2048字节)
 形参：sectorNo:扇区号
        count  :需要写入的数据长度
	   uint8_t *buffer: 参数指针
 返回：0
 详解：
************************************************************************************************/
static uint8_t FLASH_WriteSector(uint32_t sectorNo,uint16_t count,uint8_t const *buffer)
{
	uint16_t i;
	union
	{
		uint32_t  word;
		uint8_t   byte[4];
	} dest;
	dest.word = (uint32_t)(sectorNo*(1<<11));

	// 设置写入命令
	FTFL->FCCOB0 = PGM4;
	// 四字节对齐
	for(i=0;i<count;i+=4)
	{
		// 设置存储地址
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// 拷贝数据	（此数据排列按照从低位到高位存储）
		FTFL->FCCOB4 = buffer[3];
		FTFL->FCCOB5 = buffer[2];
		FTFL->FCCOB6 = buffer[1];
		FTFL->FCCOB7 = buffer[0];
		dest.word+=4; buffer+=4;
		//检测命令是否执行正常 
		if(FLASH_OK != CommandLaunch()) 
		return FLASH_ERROR;
    }
    return FLASH_OK;
}

static uint8_t flash_buffer[2048];	 //数据缓冲区
/***********************************************************************************************
 功能：写入Flash数据
 形参：FlashStartAdd:起始地址
    NumByteToWrite  :需要写入的数据长度
	        uint8_t *buffer: 参数指针
 返回：0
 详解：用户接口函数
************************************************************************************************/
void FLASH_Write(uint32_t FlashStartAdd,uint32_t NumByteToWrite,uint8_t *buffer)
{
	uint32_t i=0;
	uint32_t sectorNo = FlashStartAdd/(1<<11);        //该地址所在的扇区
	uint32_t offset =   FlashStartAdd%(1<<11);        //该地址所在扇区的偏移
	uint32_t remain =   (1<<11)-offset;               //扇区剩余大小
	if(NumByteToWrite<=remain)remain = NumByteToWrite;//不大于4096个字节
	while(1)
	{
		FLASH_Read(sectorNo*(1<<11),(1<<11),flash_buffer);  //取出该扇区所有数据
		FlashEraseSector(sectorNo);             //擦除扇区
		for(i=0;i<remain;i++)
		{
			flash_buffer[offset+i]=buffer[i];	   //修改数据
		}
		FLASH_WriteSector(sectorNo,(1<<11),flash_buffer);	//写数据
		if(NumByteToWrite == remain) break;//写入结束了
		else
		{
			sectorNo++;              //扇区地址增1
			offset=0;                //偏移位置为0 	
		  buffer+=remain;       //指针偏移
			FlashStartAdd+=remain;    //写地址偏移	  
			NumByteToWrite-=remain;				//字节数递减
			if(NumByteToWrite > (1<<11)) remain = (1<<11);	//下一个扇区还是写不完
			else remain=NumByteToWrite;			//下一个扇区可以写完了
		}
	}
}

/***********************************************************************************************
 功能：擦除一个扇区
 形参：sectorNo:扇区号
 返回：0
 详解：用户接口函数
************************************************************************************************/
uint8_t FlashEraseSector(uint32_t sectorNo)
{
	union
	{
		uint32_t  word;
		uint8_t   byte[4];
	} dest;
	dest.word = (uint32_t)(sectorNo*(1<<11));
	// 设置擦除命令
	FTFL->FCCOB0 = ERSSCR; // 擦除扇区命令
	// 设置目标地址
	FTFL->FCCOB1 = dest.byte[2];
	FTFL->FCCOB2 = dest.byte[1];
	FTFL->FCCOB3 = dest.byte[0];
	//检测命令是否执行正常
		
	if(FLASH_OK == CommandLaunch())
	{
		return FLASH_OK;
	}
	else
	{
		return FLASH_ERROR;
	}
}

//本实验定义
#define  UartX_Test uart0 
#define  WRITE_LEN  (100)  //测试写入100字节
uint8_t flashBuffer[WRITE_LEN]; //测试缓冲
float P_sbuf[5]={0};
void Flash_Test()
{
	uint8_t i;
	//开始测试
	for(i=0;i<WRITE_LEN;i++) flashBuffer[i]=i;  //赋初值
	FLASH_Write(0xB000,WRITE_LEN,flashBuffer);  //写入100扇区 0偏移 写入256字节
	for(i=0;i<WRITE_LEN;i++) UartX_printf(UartX_Test,"flashBuffer:%d\r\n",flashBuffer[i]);
	
	for(i=0;i<WRITE_LEN;i++) flashBuffer[i]=0;         //清空数据
	FLASH_Read(0xB000,WRITE_LEN,flashBuffer);  //读取数据
	for(i=0;i<WRITE_LEN;i++) UartX_printf(UartX_Test,"flashBuffer:%d\r\n",flashBuffer[i]);
	
	for(i=0;i<WRITE_LEN;i++) 
	{
		if(flashBuffer[i] != i) //数据出错
		{
			UartX_printf(UartX_Test,"FLASH TEST ERR!");
			while(1); //停机
		}
	}
	UartX_printf(UartX_Test,"FLASH TEST OK!");
	
	P_sbuf[0]=0.11;            //单独测试 写入一个数，再读出来  用指针进行操作，一个float型相当于四个八位
	FLASH_Write(0xc000 ,sizeof(P_sbuf[0]), (uint8_t *)&P_sbuf[0]);
	UartX_printf(UartX_Test,"P_sbuf0:%f\r\n",P_sbuf[0]);
	
	FLASH_Read(0xc000 ,sizeof(P_sbuf[1]), (uint8_t *)&P_sbuf[1]);
	UartX_printf(UartX_Test,"P_sbuf1:%f\r\n",P_sbuf[1]);
	while(1)  
	{		
		
	}
}


//串口,printf 函数    为了不与oled_printf冲突，自己写了一个串口重定向
//确保一次发送数据不超过USART_MAX_SEND_LEN字节
uint8_t USART_TX_BUF[200];
void UartX_printf(UARTn uartn,char* fmt,...)  
{  
	uint16_t i=0,j=0; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
			uart_putchar(uartn,USART_TX_BUF[j]); 
	} 
}
