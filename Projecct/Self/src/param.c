#include "param.h"
 
 
 
 
void paraminit()
{
	  encoder_on=TRUE;
	
	  leftPid.kp = 130;
    leftPid.ki = 15;
    leftPid.kd = 25;
    rightPid.kp = 130;
    rightPid.ki = 15;
    rightPid.kd = 25;
}
