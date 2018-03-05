#include "motor.h"
bool motor_on;
void motor_init(uint32 initvalue)
{
	motor_on=TRUE;
	ftm_pwm_init(ftm0,ftm_ch0,15000,0);
	ftm_pwm_init(ftm0,ftm_ch1,15000,initvalue);
	ftm_pwm_init(ftm0,ftm_ch2,15000,0);
	ftm_pwm_init(ftm0,ftm_ch3,15000,initvalue);
}

void motor_change(int16 motor_lf,int16 motor_rt)
{
	if(motor_on)
	{
		if(motor_lf>0)
	    ftm_pwm_duty(ftm0,ftm_ch1,motor_lf);
		else ftm_pwm_duty(ftm0,ftm_ch1,0);
		if(motor_rt>0)
	    ftm_pwm_duty(ftm0,ftm_ch3,motor_rt);
		else ftm_pwm_duty(ftm0,ftm_ch3,0);
	}
	else
	{
	ftm_pwm_duty(ftm0,ftm_ch1,0);
	ftm_pwm_duty(ftm0,ftm_ch3,0);
	}		
}
