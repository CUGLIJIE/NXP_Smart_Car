#ifndef _TYPEDEF_H
#define _TYPEDEF_H
#include "math.h"
#include "common.h"
#include "headfile.h"
#define SPEED_BUF_SIZE 5


typedef struct _img_proc_struct {
    int16_t leftBorder[40];
    int16_t rightBorder[40];
    int16_t middleLine[40];
    bool foundLeftBorder[40];
    bool foundRightBorder[40];
//    int16_t leftSlope[OV7725_H];
//    int16_t leftZero[OV7725_H];
//    int16_t rightSlope[OV7725_H];
//    int16_t rightZero[OV7725_H];
//    int16_t middleSlope[OV7725_H];
//    int16_t middleZero[OV7725_H];
//    uint16_t imgProcFlag;
	  uint16_t imgProcFlag;
    int16_t leftBorderNotFoundCnt;
    int16_t rightBorderNotFoundCnt;
}
img_proc_struct;


enum _road_type {
    Unknown,
    LeftCurve,
    RightCurve,
    CrossRoad,
	  Outside ,
	  Straight,
	  Island,
	  Island_in,
	  Island_SecondEntrance,
	  Island_out,
    Startline
};



#endif
