#include "param.h"
 
 
 
 
void paraminit()
{
	  encoder_on=TRUE;
	
	  leftPid.kp = 125;
    leftPid.ki = 20;
    leftPid.kd = 15;
    rightPid.kp = 125;
    rightPid.ki = 20;
    rightPid.kd = 15;
}
