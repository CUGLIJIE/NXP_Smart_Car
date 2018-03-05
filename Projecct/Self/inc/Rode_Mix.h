#ifndef _RODE_MIX_h
#define _RODE_MIX_h

#include "common.h"

#define  STRAIGHT_ROAD  0x0001
#define  CROSS_ROAD     0x0002
#define  LEFTCUVER      0x0003
#define  RIGHTCUVER     0x0004
void Summarycontrol(void);
uint8_t get_rode_type(void);
#endif



