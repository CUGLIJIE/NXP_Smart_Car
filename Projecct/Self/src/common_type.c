#include "common_type.h"
#include "Imageprocess.h"
int16_t startLineWidth=100;
uint8_t OutsideCount;
/**********************����ͣ��*******************/
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

/*************************������ʶ��*******************************
������ʶ��  ����3��ͼ��ļ�������������5�β���ǰհ������10������ 
8����9�����ϵ�ʵ����������������25�м���ǰհǰ���������ֱ����������������������������������
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

/**************ֱ��ʶ��********************************************************
******����40�����ڵļ�������߷�Χ�г���25����188/2������25������**************
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
