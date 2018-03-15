
//���ݳ��˿�2.3�汾�޸Ķ���������ʵ�ֲ������籣��

#include "Flash_Date.h"
#include "MK60D10.h"
#include "stdint.h"
#include "MK60_uart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"	 
/***********************************************************************************************
 ���ܣ��ڲ����� ��������Ƿ����
 �βΣ�0
 ���أ�FLASH_OK �ɹ�    FLASH_ERROR ʧ�� 
 ��⣺
************************************************************************************************/
static uint8_t CommandLaunch(void)
{
    // ������ʴ����־λ�ͷǷ����ʱ�־λ 
    FTFL->FSTAT |=(FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    // ��������
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;
    // �ȴ��������
    while((FTFL->FSTAT &FTFL_FSTAT_CCIF_MASK)==0);
    // �������־
    if(FTFL->FSTAT & (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_MGSTAT0_MASK|FTFL_FSTAT_RDCOLERR_MASK))
    return(FLASH_ERROR); //����
    return (FLASH_OK); //�ɹ�
}

/***********************************************************************************************
 ���ܣ���ʼ���ڲ�Flashģ��
 �βΣ�0
 ���أ�0
 ��⣺
************************************************************************************************/
void FLASH_Init(void)
{
	//���FMC������
	FMC->PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;
	FMC->PFB1CR |= FMC_PFB0CR_S_B_INV_MASK;
	// ��ֹ���Ź�
	WDOG->UNLOCK = 0xC520;
	WDOG->UNLOCK = 0xD928;
	WDOG->STCTRLH = 0;    // ��ֹ���Ź�
	//���Flash���ʴ���
  if(FTFL->FSTAT & FTFL_FSTAT_ACCERR_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_ACCERR_MASK;       //��������־
  }
  //��鱣������
  else if (FTFL->FSTAT & FTFL_FSTAT_FPVIOL_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_FPVIOL_MASK;
  }
  //������ͻ����
  else if (FTFL->FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;
  }
  //����Flashģ������ݻ���
  FMC->PFB0CR &= ~FMC_PFB0CR_B0DCE_MASK;
  FMC->PFB1CR &= ~FMC_PFB1CR_B1DCE_MASK;
}
/***********************************************************************************************
 ���ܣ���ȡFlash����
 �βΣ�start_addr: ��ʵ��ַ  
             len: Ҫ��ȡ�ĳ���
		 uint8_t *pbuffer: ����ָ��
 ���أ�0
 ��⣺
************************************************************************************************/
void FLASH_Read(uint32_t FlashStartAdd,uint32_t len,uint8_t *pbuffer)
{
	uint32_t i=0;
	for(i=0;i<len;i++)
	{
	  pbuffer[i]=*(uint8_t *)(FlashStartAdd+i);		//��ȡָ����ַ������
	}
}

/***********************************************************************************************
 ���ܣ�дһ������(һ������2048�ֽ�)
 �βΣ�sectorNo:������
        count  :��Ҫд������ݳ���
	   uint8_t *buffer: ����ָ��
 ���أ�0
 ��⣺
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

	// ����д������
	FTFL->FCCOB0 = PGM4;
	// ���ֽڶ���
	for(i=0;i<count;i+=4)
	{
		// ���ô洢��ַ
		FTFL->FCCOB1 = dest.byte[2];
		FTFL->FCCOB2 = dest.byte[1];
		FTFL->FCCOB3 = dest.byte[0];
		// ��������	�����������а��մӵ�λ����λ�洢��
		FTFL->FCCOB4 = buffer[3];
		FTFL->FCCOB5 = buffer[2];
		FTFL->FCCOB6 = buffer[1];
		FTFL->FCCOB7 = buffer[0];
		dest.word+=4; buffer+=4;
		//��������Ƿ�ִ������ 
		if(FLASH_OK != CommandLaunch()) 
		return FLASH_ERROR;
    }
    return FLASH_OK;
}

static uint8_t flash_buffer[2048];	 //���ݻ�����
/***********************************************************************************************
 ���ܣ�д��Flash����
 �βΣ�FlashStartAdd:��ʼ��ַ
    NumByteToWrite  :��Ҫд������ݳ���
	        uint8_t *buffer: ����ָ��
 ���أ�0
 ��⣺�û��ӿں���
************************************************************************************************/
void FLASH_Write(uint32_t FlashStartAdd,uint32_t NumByteToWrite,uint8_t *buffer)
{
	uint32_t i=0;
	uint32_t sectorNo = FlashStartAdd/(1<<11);        //�õ�ַ���ڵ�����
	uint32_t offset =   FlashStartAdd%(1<<11);        //�õ�ַ����������ƫ��
	uint32_t remain =   (1<<11)-offset;               //����ʣ���С
	if(NumByteToWrite<=remain)remain = NumByteToWrite;//������4096���ֽ�
	while(1)
	{
		FLASH_Read(sectorNo*(1<<11),(1<<11),flash_buffer);  //ȡ����������������
		FlashEraseSector(sectorNo);             //��������
		for(i=0;i<remain;i++)
		{
			flash_buffer[offset+i]=buffer[i];	   //�޸�����
		}
		FLASH_WriteSector(sectorNo,(1<<11),flash_buffer);	//д����
		if(NumByteToWrite == remain) break;//д�������
		else
		{
			sectorNo++;              //������ַ��1
			offset=0;                //ƫ��λ��Ϊ0 	
		  buffer+=remain;       //ָ��ƫ��
			FlashStartAdd+=remain;    //д��ַƫ��	  
			NumByteToWrite-=remain;				//�ֽ����ݼ�
			if(NumByteToWrite > (1<<11)) remain = (1<<11);	//��һ����������д����
			else remain=NumByteToWrite;			//��һ����������д����
		}
	}
}

/***********************************************************************************************
 ���ܣ�����һ������
 �βΣ�sectorNo:������
 ���أ�0
 ��⣺�û��ӿں���
************************************************************************************************/
uint8_t FlashEraseSector(uint32_t sectorNo)
{
	union
	{
		uint32_t  word;
		uint8_t   byte[4];
	} dest;
	dest.word = (uint32_t)(sectorNo*(1<<11));
	// ���ò�������
	FTFL->FCCOB0 = ERSSCR; // ������������
	// ����Ŀ���ַ
	FTFL->FCCOB1 = dest.byte[2];
	FTFL->FCCOB2 = dest.byte[1];
	FTFL->FCCOB3 = dest.byte[0];
	//��������Ƿ�ִ������
		
	if(FLASH_OK == CommandLaunch())
	{
		return FLASH_OK;
	}
	else
	{
		return FLASH_ERROR;
	}
}

//��ʵ�鶨��
#define  UartX_Test uart0 
#define  WRITE_LEN  (100)  //����д��100�ֽ�
uint8_t flashBuffer[WRITE_LEN]; //���Ի���
float P_sbuf[5]={0};
void Flash_Test()
{
	uint8_t i;
	//��ʼ����
	for(i=0;i<WRITE_LEN;i++) flashBuffer[i]=i;  //����ֵ
	FLASH_Write(0xB000,WRITE_LEN,flashBuffer);  //д��100���� 0ƫ�� д��256�ֽ�
	for(i=0;i<WRITE_LEN;i++) UartX_printf(UartX_Test,"flashBuffer:%d\r\n",flashBuffer[i]);
	
	for(i=0;i<WRITE_LEN;i++) flashBuffer[i]=0;         //�������
	FLASH_Read(0xB000,WRITE_LEN,flashBuffer);  //��ȡ����
	for(i=0;i<WRITE_LEN;i++) UartX_printf(UartX_Test,"flashBuffer:%d\r\n",flashBuffer[i]);
	
	for(i=0;i<WRITE_LEN;i++) 
	{
		if(flashBuffer[i] != i) //���ݳ���
		{
			UartX_printf(UartX_Test,"FLASH TEST ERR!");
			while(1); //ͣ��
		}
	}
	UartX_printf(UartX_Test,"FLASH TEST OK!");
	
	P_sbuf[0]=0.11;            //�������� д��һ�������ٶ�����  ��ָ����в�����һ��float���൱���ĸ���λ
	FLASH_Write(0xc000 ,sizeof(P_sbuf[0]), (uint8_t *)&P_sbuf[0]);
	UartX_printf(UartX_Test,"P_sbuf0:%f\r\n",P_sbuf[0]);
	
	FLASH_Read(0xc000 ,sizeof(P_sbuf[1]), (uint8_t *)&P_sbuf[1]);
	UartX_printf(UartX_Test,"P_sbuf1:%f\r\n",P_sbuf[1]);
	while(1)  
	{		
		
	}
}


//����,printf ����    Ϊ�˲���oled_printf��ͻ���Լ�д��һ�������ض���
//ȷ��һ�η������ݲ�����USART_MAX_SEND_LEN�ֽ�
uint8_t USART_TX_BUF[200];
void UartX_printf(UARTn uartn,char* fmt,...)  
{  
	uint16_t i=0,j=0; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
			uart_putchar(uartn,USART_TX_BUF[j]); 
	} 
}
