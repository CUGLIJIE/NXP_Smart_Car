#include "Encoder.h"
#include "Island.h"

bool encoder_on;
void distancecontrol(void);

void EncoderInit() 
	{
    ftm_quad_init(ftm2);
	  lptmr_pulse_init(lpt0_alt2,0xFFFF,LPT_Falling);
		gpio_init(C6,GPI,0);	
    gpio_ddr(C6,GPI);	
	}

void EncoderGet(int16_t* left, int16_t* right) 
{
			*left = lptmr_pulse_get();	
    if(gpio_get(C6)) {
        *right = -ftm_quad_get(ftm2);
    } else {
        *right = ftm_quad_get(ftm2);
    }
    EncoderClear();
}

void EncoderClear() 
{
    ftm_quad_clean(ftm2);
    lptmr_pulse_clean();	
}




void MainProc() {
  
    if(encoder_on) {
        EncoderGet(&leftSpeed, &rightSpeed);
    } else {
        EncoderClear();
        leftSpeed = rightSpeed = 0;
    }
     distancecontrol();

    SpeedControlProc(leftSpeed, rightSpeed);
    
}


//�������Ǿ��� 
void distancecontrol(){
     int16_t dist = (leftSpeed + rightSpeed) / 2;
	  if(IslandDistanceCountFlag){
		     IslandDistance += dist;
		}	
}



