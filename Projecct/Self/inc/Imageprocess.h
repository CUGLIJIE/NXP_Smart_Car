#ifndef _Imageprocess_h
#define _Imageprocess_h
#include "common.h"
#include "headfile.h"
#include "SEEKFREE_MT9V032.h"
//#define COL_replace COL
//#define ROW_replace ROW
////#define COL_replace=COL;
////#define ROW_replace=ROW;


void binaryzation(uint8 *image_b);
//void   seekfree_sendimg_032(void);
extern uint8   image_binary[60][188]; 

#endif
