#include "headfile.h"
#include "Island.h"
#include "Flash_Date.h"

extern _Island_ LeftIsland;
extern _Island_ RightIsland;

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
//				seekfree_sendimg_032();
				ImgProc();
//				translate_image();
//				LeftIsland.anotherslope[0]=R_Slope_Figure(6);
//				RightIsland.anotherslope[0]=L_Slope_Figure(6);
//				UartX_printf(uart1,"左边界斜率=%f  右边界斜率=%f  \r\n",fabs(RightIsland.anotherslope[0]),LeftIsland.anotherslope[0]);	
				
				mt9v032_finish_flag = 0;
			}			
	}
}
