#ifndef _motor_H
#define _motor_H

#include "headfile.h"
void motor_init(uint32 initvalue);
void motor_change(int16 motor_lf,int16 motor_rt);
extern bool motor_on;
#endif
