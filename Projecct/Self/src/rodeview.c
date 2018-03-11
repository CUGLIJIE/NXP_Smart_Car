#include "rodeview.h"
#include "Rode_Mix.h"
#include "Island.h"
img_proc_struct resultSet;


int16_t searchForBordersStartIndex_lastimg=-1;
void ImgProc() 
{	
		int16_t searchForBordersStartIndex;
		int16_t searchstartline=39,searchendline=0;
		if(searchForBordersStartIndex_lastimg==-1)
		{
			searchForBordersStartIndex = COL / 2;
		}
		else searchForBordersStartIndex =searchForBordersStartIndex_lastimg;
				
    for(int16_t row = searchstartline; row >searchendline-1 ; --row) 
		{
        resultSet.foundLeftBorder[row] =
        LeftBorderSearchFrom(row, searchForBordersStartIndex);
        resultSet.foundRightBorder[row] =
        RightBorderSearchFrom(row, searchForBordersStartIndex);
        MiddleLineUpdate(row);
        searchForBordersStartIndex = resultSet.middleLine[row];
    }

		int16_t middle = COL / 2;
		Summarycontrol(); 
		DirectionControlProc(resultSet.middleLine, middle);
		SpeedTargetSet(resultSet.imgProcFlag);
}


