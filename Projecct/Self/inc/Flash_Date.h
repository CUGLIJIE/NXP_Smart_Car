/***********************************************************************************************
//CH_Kinetis驱动库  V2.3
//作者    :YANDLD 
//E-MAIL  :yandld@126.com
//修改日期:2013/2/14
//版本：V2.3
//淘宝：http://upcmcu.taobao.com
//QQ    1453363089
//Copyright(C) YANDLD 2012-2022
//All rights reserved
************************************************************************************************/
#ifndef __FLASH_H__
#define __FLASH_H__
#include "MK60D10.h"
#include "MK60_uart.h"
#include "stdint.h"

//Flash命令宏定义，内部使用
#define RD1BLK    0x00   // 读整块Flash
#define RD1SEC    0x01   // 读整个扇区
#define PGMCHK    0x02   // 写入检查
#define RDRSRC    0x03   // 读目标数据
#define PGM4      0x06   // 写入长字
#define ERSBLK    0x08   // 擦除整块Flash
#define ERSSCR    0x09   // 擦除Flash扇区
#define PGMSEC    0x0B   // 写入扇区
#define RD1ALL    0x40   // 读所有的块
#define RDONCE    0x41   // 只读一次
#define PGMONCE   0x43   // 只写一次
#define ERSALL    0x44   // 擦除所有块
#define VFYKEY    0x45   // 验证后门访问钥匙
#define PGMPART   0x80   // 写入分区
#define SETRAM    0x81   // 设定FlexRAM功能

//错误代码定义
#define FLASH_OK                    0x00
#define FLASH_OVERFLOW              0x01
#define FLASH_BUSY                  0x02
#define FLASH_ERROR                 0x04
#define FLASH_TIMEOUT               0x08
#define FLASH_NOT_ERASED            0x10
#define FLASH_CONTENTERR            0x11

//本构件所实现的函数接口
void FLASH_Init(void);
void FLASH_Read(uint32_t FlashStartAdd,uint32_t len,uint8_t *pbuffer);
void FLASH_Write(uint32_t FlashStartAdd,uint32_t NumByteToWrite,uint8_t *buffer);
uint8_t FlashEraseSector(uint32_t sectorNo);
void Flash_Test(void);

void UartX_printf(UARTn uartn,char* fmt,...);
#endif
