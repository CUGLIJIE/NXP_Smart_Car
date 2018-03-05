#ifndef _rodeview_h
#define _rodeview_h
#include "common.h"
#include "Typedef.h"
#include "headfile.h"
#include "SEEKFREE_MT9V032.h"
#include "Imageprocess.h"
extern img_proc_struct resultSet;

void ImgProc(void);
extern int16_t searchForBordersStartIndex_lastimg;

extern inline int16_t Abs(int16_t input) {
    return input >= 0 ? input : -input;
}

extern inline int16_t Min(int16_t a, int16_t b) {
    return a > b ? b : a;
}

extern inline int16_t Max(int16_t a, int16_t b) {
    return a > b ? a : b;
}

extern inline float Min_f(float a, float b) {
    return a > b ? b : a;
}

extern inline float Max_f(float a, float b) {
    return a > b ? a : b;
}

//extern inline bool OpstSign(int16_t a, int16_t b) {
//    return a * b < 0;
//}

extern inline bool InRange(int16_t value, int16_t lbound, int16_t hbound) {
	return value > lbound && value < hbound?TRUE:FALSE;
}

extern inline bool InRange_f(float value, float lbound, float hbound) {
    return value > lbound && value < hbound?TRUE:FALSE;
}

extern inline int16_t Limit(int16_t value, int16_t lbound, int16_t hbound) {
    return value > hbound ? hbound : value < lbound ? lbound : value;
}

extern inline float Limit_f(float value, float lbound, float hbound) {
    return value > hbound ? hbound : value < lbound ? lbound : value;
}






#endif
