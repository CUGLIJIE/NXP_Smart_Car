#include "Imageprocess.h"

//#define COL_replace COL
//#define ROW_replace ROW

uint8   image_binary[ROW][COL];  
uint8   threshold_binary=140;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ¶þÖµ»¯
//-------------------------------------------------------------------------------------------------------------------
void binaryzation(uint8 *image_b)
{
	uint8 row_b=0,col_b=0;
	while(row_b<ROW-1)
	{
		if(*image_b>threshold_binary)
		{
			image_binary[row_b][col_b]=0xff;
		}
		else 
			image_binary[row_b][col_b]=0;
		  col_b++;
		if(col_b>COL-1)
		{
			col_b=0;
			row_b++;
		}
		image_b++;
 }
}


