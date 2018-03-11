#include "common_type.h"
#include "Imageprocess.h"
int16_t startLineWidth=100;
uint8_t OutsideCount;
/**********************出界停车*******************/
 bool IsOutOfRoad() {
    uint16 cnt = 0;
     
	  for(uint8 row = 55; row >53; --row )
	  {
        for(uint8 col = 50; col < 140; ++col) 
	     {
					if(image_binary[row][col]==0x00) 
						{
							++cnt;
						}
        }
		}
        if(cnt >170){
					++OutsideCount;
				
				}	
				if(OutsideCount>=7)
				{
					OutsideCount=0;
					return TRUE;
				}
        else 
				return FALSE;
				
}

/*************************起跑线识别*******************************
起跑线识别  至少3幅图像的检测跳变次数大于5次并且前瞻的上面10行下面 
8行有9次以上的实际中线在中线左右25列即在前瞻前后的赛道是直道················
*******************************************************************/
bool IsStartLine(int16_t presight) {
    int16_t toggleCnt = 0;
    int16_t patternRowCnt = 0;
    for(int16_t i = presight; i < presight + 4; ++i) {
        for(int16_t j = COL / 2 - startLineWidth / 2; j < COL / 2 + startLineWidth / 2; ++j) {
            if(image_binary[i][j]!= image_binary[i][j+1]) {
                if(++toggleCnt > 5) {
                    toggleCnt = 0;
                     ++patternRowCnt;
                }
            }
        }
    }
		
    if(patternRowCnt >= 3) {
        int16_t middleAreaCnt = 0;
        for(int16_t i = 0; i < presight - 4; ++i) {
            if(InRange(resultSet.middleLine[i], COL / 2 - 25, COL / 2 + 25)) {
                ++middleAreaCnt;
            }
        }
        for(int16_t i = presight + 10; i < ROW; ++i) {
            if(InRange(resultSet.middleLine[i], COL / 2 - 25, COL / 2 + 25)) {
                ++middleAreaCnt;
            }
        }
				
        if( middleAreaCnt > 5)
					return TRUE;
				   else return FALSE;				
      } 	
			else {
        return FALSE;
    }
}

/**************直道识别********************************************************
******根据40行以内的计算的中线范围有超过25次在188/2的左右25列以内**************
*******************************************************************************/
bool IsStraightLine(void) {
    int16_t middleAreaCnt = 0;
    for(int16_t i = 0; i < ROW-20; ++i) {
        if(InRange(resultSet.middleLine[i], COL / 2 - 5, COL / 2 + 5)) {
            ++middleAreaCnt;
        }
    }
		if(middleAreaCnt>30){		
		 return TRUE;	
		}
		else return FALSE;      
}
