#include "motor.h"
bool motor_on;
void motor_init(uint32 initvalue)
{
	motor_on=TRUE;
	ftm_pwm_init(ftm0,ftm_ch0,10000,initvalue);
	ftm_pwm_init(ftm0,ftm_ch1,10000,initvalue);
	ftm_pwm_init(ftm0,ftm_ch2,10000,initvalue);
	ftm_pwm_init(ftm0,ftm_ch3,10000,initvalue);
}

void motor_change(int16 motor_lf,int16 motor_rt)
{
	if(motor_on)
	{   
		motor_lf = Limit(motor_lf, -10000, 10000);
    motor_rt = Limit(motor_rt, -10000, 10000);
		if(motor_lf>0)
		{
	    ftm_pwm_duty(ftm0,ftm_ch1,0);
		  ftm_pwm_duty(ftm0,ftm_ch0,motor_lf);
		}
   else 
      {
		  ftm_pwm_duty(ftm0,ftm_ch1,-motor_lf);
		  ftm_pwm_duty(ftm0,ftm_ch0,0);
			}
	if(motor_rt>0)
		{
			ftm_pwm_duty(ftm0,ftm_ch2,motor_rt);
      ftm_pwm_duty(ftm0,ftm_ch3,0);
	  }
	    
		 else 
			 {
		   		ftm_pwm_duty(ftm0,ftm_ch2,0);
          ftm_pwm_duty(ftm0,ftm_ch3,-motor_rt);
			 }   
	}
	else{
	    ftm_pwm_duty(ftm0,ftm_ch1,0);
		  ftm_pwm_duty(ftm0,ftm_ch0,0);
		  ftm_pwm_duty(ftm0,ftm_ch2,0);
      ftm_pwm_duty(ftm0,ftm_ch3,0);
	
	}
	
}
