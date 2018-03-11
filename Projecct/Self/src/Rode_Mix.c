#include "Rode_Mix.h"
#include "Cuver.h"
#include "CrossRode.h"
#include "Typedef.h"
#include "common_type.h"
#include "Island.h"
extern uint16_t speedcontrol;
extern uint8_t OutsideCount;

extern _Island_ LeftIsland;
extern _Island_ RightIsland;

uint8_t get_rode_type()
{
	if((!LeftIsland.f_IslandIn||!RightIsland.f_IslandIn)&&IslandStep<=2)//正常路径下
	{
		return IsOutOfRoad()?Outside:IsStartLine(pre_sight)?Startline:IslandfristEntrancejudge()?Island_in:IsCrossRoad()?CrossRoad:IsStraightLine()?Straight:WhichCurve() ;	 
	}
	else if((LeftIsland.f_IslandIn||RightIsland.f_IslandIn)&&IslandStep==3)//识别到环岛入口，准备识别第二个入口
	{
//		OLEDPrintf(0,7, "10") ;
		return IslandsecondEntrancejudge()?Island_SecondEntrance:FALSE;
		
	}	
	else if((LeftIsland.f_IslandIn||RightIsland.f_IslandIn)&&IslandStep==4)//进入环岛后，准备识别出口
	{
//		OLEDPrintf(50,7, "50") ;
		return IslandExitjudge()?Island_out:FALSE;
	}
	else if((LeftIsland.f_IslandIn||RightIsland.f_IslandIn)&&IslandStep==5)//离开出口，准备再次识别第二个入口
	{
//		OLEDPrintf(100,7, "100") ;
		return IslandsecondEntrancejudge()?Island_SecondEntrance:FALSE;
	}
	else
	{
		return FALSE;	
	}
}
void Summarycontrol(){
	
    switch(get_rode_type()){
		 
		 case Outside :  
									OLEDPrintf(1,6, "Outside") ;
									motor_on=FALSE; ; 
		 break;	
		 case Startline:
								  OLEDPrintf(1,6, "STARTLINE") ;
								  motor_on=TRUE;  ; 
		 break;
     case CrossRoad:	 
								  OLEDPrintf(1,6, "CR+OSSRODE") ;
								  resultSet.imgProcFlag |= CROSS_ROAD;
								  CrossRoadAction() ; 
		 break;
		 case  Island_in:  
										speedcontrol=60;
		 break;	
     case  Island_SecondEntrance:
							    IslandsecondEntranceProc();		
	   break;
		 case  Island_out:
					        IslandExitProc();				 
									
	   break;
			 
		 case Straight:
									BUZZLE_OFF;
                  OutsideCount=0;//防止意外停车		 
//									OLEDPrintf(1,6, "Straight") ;
									resultSet.imgProcFlag |= STRAIGHT_ROAD; 
		 break;  
		 case LeftCurve:
//									OLEDPrintf(1,6, "LeftCurve") ;  
									resultSet.imgProcFlag |= LEFTCUVER;
//							    LeftCurveAction();  
		 break;								 
		 case RightCurve:		 
//									OLEDPrintf(1,6, "RightCurve") ;
								  resultSet.imgProcFlag |= RIGHTCUVER; 
//							    RightCurveAction();  
		 break;
          
									
		 default: break;		 	 
  }
	 
}


