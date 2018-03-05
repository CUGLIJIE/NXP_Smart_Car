#include "headfile.h"
#include "Island.h"

void BuzzleInit()
{
	 gpio_init(BUZZLE_PORT_PIN,GPO,1);
	 BUZZLE_OFF;
}

int main(void)
{

    get_clk();
//  uart_init (uart3, 512000);
    uart_init (uart1, 115200);
    camera_init();
	  OLED_Init();
	  BuzzleInit();
	  SteerActuatorInit();
    motor_init(0);
	  EncoderInit();
	  paraminit();
	  pit_init_ms(pit0,5);
    enable_irq(PIT0_IRQn);
    EnableInterrupts;
	  		
	
	for(;;)
	{
		if(mt9v032_finish_flag)
			{
				binaryzation((uint8_t *)image);
				//seekfree_sendimg_032();
				ImgProc();
//				IslandOutjudge();
//				IslandOutProc();
//  
//				 translate_image();
				
				mt9v032_finish_flag = 0;
			}			
	}
}
