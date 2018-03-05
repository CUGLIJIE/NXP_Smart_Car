#include "CrossRode.h"
#include "rodeview.h"

#include "BorderSearch.h"
int32_t crossRoadDistance;
int32_t crossRoadDistanceMax;

bool inCrossRoad;
int16_t crossRoadOvertakingCnt;
/*********************Ê®×ÖÅÐ¶Ï******************/
bool IsCrossRoad() {
    int16_t cnt = 0;
    for(int16_t i = 5; i < 30; ++i) {
        if(!resultSet.foundLeftBorder[i] && !resultSet.foundRightBorder[i]) {
            ++cnt;
        }
    }
    if(cnt > 9) {
        cnt = 0;
        for(int16_t i = 0; i < 40; ++i) {
            if(IsBlack(i, resultSet.middleLine[i])) {
                ++cnt;
            }
        }
        return (bool)(cnt < 5);
    } else {
        return FALSE;
    }
}


/*******************Ê®×Ö²¹Ïß*************************************/
void CrossRoadAction() {
	
    int16_t col;
    int16_t left;
    int16_t right;
    int16_t middle;
	
	
    for(col = COL / 2; col > 0 && IsWhite( 4, col); --col) { }
       for(; col < COL /2 + 40 &&   IsBlack( 4, col); ++col) { }
        if(col == COL / 2 + 40) {
          --col;
          for(; col > COL / 2 - 40 && IsBlack( 4, col); --col) { }
          if(col == COL / 2 - 40) {
            return;
        } 
					
				
				else {
					
            right = col;
            for(; col > 0 && IsWhite( 4, col); --col) { }
            left = col;
        }
    } 	
				
				
		   else {
        left = col;
        for(; col < COL - 1 && IsWhite(4, col); ++col) { }
        right = col;
				
    }
			 
		
    middle = (left + right) / 2;	
    for(int16_t row = pre_sight - 3; row < pre_sight + 3; ++row) {
         resultSet.middleLine[row] = (resultSet.middleLine[5] + middle) / 2;
			
   }
}



