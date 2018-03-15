/***********************************************************************************************
//CH_Kinetis������  V2.3
//����    :YANDLD 
//E-MAIL  :yandld@126.com
//�޸�����:2013/2/14
//�汾��V2.3
//�Ա���http://upcmcu.taobao.com
//QQ    1453363089
//Copyright(C) YANDLD 2012-2022
//All rights reserved
************************************************************************************************/
#ifndef __FLASH_H__
#define __FLASH_H__
#include "MK60D10.h"
#include "MK60_uart.h"
#include "stdint.h"

//Flash����궨�壬�ڲ�ʹ��
#define RD1BLK    0x00   // ������Flash
#define RD1SEC    0x01   // ����������
#define PGMCHK    0x02   // д����
#define RDRSRC    0x03   // ��Ŀ������
#define PGM4      0x06   // д�볤��
#define ERSBLK    0x08   // ��������Flash
#define ERSSCR    0x09   // ����Flash����
#define PGMSEC    0x0B   // д������
#define RD1ALL    0x40   // �����еĿ�
#define RDONCE    0x41   // ֻ��һ��
#define PGMONCE   0x43   // ֻдһ��
#define ERSALL    0x44   // �������п�
#define VFYKEY    0x45   // ��֤���ŷ���Կ��
#define PGMPART   0x80   // д�����
#define SETRAM    0x81   // �趨FlexRAM����

//������붨��
#define FLASH_OK                    0x00
#define FLASH_OVERFLOW              0x01
#define FLASH_BUSY                  0x02
#define FLASH_ERROR                 0x04
#define FLASH_TIMEOUT               0x08
#define FLASH_NOT_ERASED            0x10
#define FLASH_CONTENTERR            0x11

//��������ʵ�ֵĺ����ӿ�
void FLASH_Init(void);
void FLASH_Read(uint32_t FlashStartAdd,uint32_t len,uint8_t *pbuffer);
void FLASH_Write(uint32_t FlashStartAdd,uint32_t NumByteToWrite,uint8_t *buffer);
uint8_t FlashEraseSector(uint32_t sectorNo);
void Flash_Test(void);

void UartX_printf(UARTn uartn,char* fmt,...);
#endif
