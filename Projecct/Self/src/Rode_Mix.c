#include "Rode_Mix.h"
#include "Cuver.h"
#include "CrossRode.h"
#include "Typedef.h"
#include "common_type.h"
#include "Island.h"
bool IslandInflag=FALSE ;
extern uint16_t speedcontrol;

uint8_t get_rode_type(){
	if(IslandInflag==FALSE)
	{
		return IsOutOfRoad()?Outside:IsStartLine(pre_sight)?Startline:Island_judge()?Island_in:IsCrossRoad()?CrossRoad:IsStraightLine()?Straight:WhichCurve() ;	 
	}
	else if(Islandleaveflag==TRUE)
	{
		return IslandOut_judge()?Island_out:FALSE;
	}
	else if(IslandInflag==TRUE&&Islandleaveflag==FALSE)
	{
		return IslandSecondEntrance_judge()?Island_SecondEntrance:IslandOut_judge()?Island_out:FALSE;
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
//									speedcontrol=60;
//								  BUZZLE_ON;
									OLEDPrintf(1,6, "Island_in") ;
									IslandInflag=TRUE ;
									IslandDistanceCountFlag=TRUE;
									island_entrance=FALSE;				
		 break;	
     case  Island_SecondEntrance:
									IslandSecondEntranceProc();
	   break;
		 case  Island_out:
									 if(IslandInflag&&!IslandleaveDistanceCountFlag)
									 {
										 IslandOutProc();
									 }
									 else  if(IslandInflag&&IslandleaveDistanceCountFlag)
									 {
										 ;
									 }
									
	   break;
			 
		 case Straight:
									BUZZLE_OFF;	
									speedcontrol=75;		 
									OLEDPrintf(1,6, "Straight") ;
									resultSet.imgProcFlag |= STRAIGHT_ROAD; 
		 break;  
		 case LeftCurve:
									if(IslandInflag) 
									{
										motor_on=TRUE;
									}			
									OLEDPrintf(1,6, "LeftCurve") ;  
									resultSet.imgProcFlag |= LEFTCUVER;
//							    LeftCurveAction();  
		 break;								 
		 case RightCurve:
									if(IslandInflag) 
									{
										motor_on=TRUE;
									}				 
									OLEDPrintf(1,6, "RightCurve") ;
								  resultSet.imgProcFlag |= RIGHTCUVER; 
//							    RightCurveAction();  
		 break;
          
									
		 default:BUZZLE_OFF;break;		 	 
  }
	 
}


