#include "Island.h"
#include "rodeview.h"
uint8 Roundaboutstep=0;
uint8 Roundabout=0;
uint8 jumping=0; //环岛进程需用
bool island_entrance;
int32_t IslandDistance;
int32_t IslandOutDistance;
int32_t IslandleaveDistance;
extern bool IslandInflag;
extern uint16_t speedcontrol;

bool Islandleaveflag=FALSE;

uint8_t leftjumpout=0;
uint8_t leftmidblackcol=0;
uint8_t leftmidblackrow=0;
uint8_t leftmidtemp=0;
bool leftjumpoutflag=FALSE;
bool leftmidblackflag=FALSE;
bool leftIslandOutflag=FALSE;
bool IslandOutDistanceCountFlag=FALSE ;
bool IslandleaveDistanceCountFlag=FALSE ;


uint8_t SecondEntranceInflexionRow=0;
uint8_t SecondEntranceInflexionCol=0;
bool    SecondEntranceInflexionFlag=FALSE;
bool IslandDistanceCountFlag=FALSE ;

void IsRoundabout()
{
	uint8 anothersmooth=0;
	uint8 anotherstraight=0;	
	float anotherxielv1=0;
	float anotherxielv2=0;
	float anotherxielv3=0;	
	uint8 midblack=0;
	uint8 middleblack[8]={0};
	float midblacksum=0;
	uint8 mblacknumber=0;	
	uint8 farwhite=0;
	uint8 farwhitesum=0;
	uint8 farwhitemid=0;	
	uint8 IsRoundstep=0;		
  jumping=0;
	for(uint8 r=20;r>4;r--)
	{
		if(resultSet.rightBorder[r-2]-resultSet.rightBorder[r] >20)
		{
			IsRoundstep=1;
			jumping=r;
//			OLEDPrintf(1, 2, "%d",jumping);
//		OLEDPrintf(1, 3, "%d  %d",resultSet.rightBorder[r-2],resultSet.rightBorder[r]) ;
			break;
		}
  }
//   OLEDPrintf(1, 5, "%d  %d %d",resultSet.leftBorder[12],(resultSet.leftBorder[12]+resultSet.rightBorder[12])/2,resultSet.rightBorder[12]) ;
	
	//条件2：另一边无跳变
	if(jumping!=0)
	{
		anothersmooth=1;
		IsRoundstep=2;
		for(uint8 r=39;r>4;r--)
		{
			if((resultSet.leftBorder[r]-resultSet.leftBorder[r-2] >10))//||(resultSet.leftBorder[r-2]-resultSet.leftBorder[r]<-5))
			{
				IsRoundstep=1;
				anothersmooth=0;
				break;
			}
		}
	}

	//	//条件3：另一边直道
	if(anothersmooth==1)
	{
		anotherstraight=1;
		IsRoundstep=3;
    anotherxielv1=(float)(resultSet.leftBorder[7]+resultSet.leftBorder[8]+resultSet.leftBorder[9]-resultSet.leftBorder[15]-resultSet.leftBorder[16]-resultSet.leftBorder[17])/24;
    anotherxielv2=(float)(resultSet.leftBorder[14]+resultSet.leftBorder[15]+resultSet.leftBorder[16]-resultSet.leftBorder[24]-resultSet.leftBorder[25]-resultSet.leftBorder[26])/30;
    anotherxielv3=(float)(resultSet.leftBorder[25]+resultSet.leftBorder[26]+resultSet.leftBorder[27]-resultSet.leftBorder[35]-resultSet.leftBorder[36]-resultSet.leftBorder[37])/30;
//		OLEDPrintf(1, 3, "%f",anotherxielv1);
//		OLEDPrintf(1, 4, "%f",anotherxielv2);
//		OLEDPrintf(1, 5, "%f",anotherxielv3);
		if(abs(anotherxielv1-anotherxielv2)>0.3||abs(anotherxielv1-anotherxielv3)>0.3||abs(anotherxielv2-anotherxielv3)>0.3)
			{
				IsRoundstep=2;
				anotherstraight=0;
			}
	}
		//条件4：环岛中间黑块
	if(anotherstraight==1)
	{
		midblack=1;
		for(uint8 c=resultSet.rightBorder[jumping];c<resultSet.rightBorder[jumping]+8;c++)
			for(uint8 r=jumping-3;r>2;r--)
			{
				if(image_binary[r][c]==0x00){middleblack[c-resultSet.rightBorder[jumping]]=r;break;}
			}
//						OLEDPrintf(1, 3, "%d %d %d %d",middleblack[0],middleblack[1],middleblack[2],middleblack[3]);
//						OLEDPrintf(1, 4, "%d %d %d %d",middleblack[4],middleblack[5],middleblack[6],middleblack[7]);
			for(uint8 i=0;i<8;i++)
			{
				if(middleblack[i]>0)
				{
					if(abs(middleblack[i]-middleblack[3])>2){midblack=0; break;}
				  midblacksum+=middleblack[i];
				}
				else {mblacknumber++;}	
			}
			if(mblacknumber<3)
			{
				midblacksum=midblacksum/(8-mblacknumber);
			if(midblacksum>5||midblacksum<3){midblack=0;}
		  }
			else midblack=0;
//				OLEDPrintf(1, 3, "%f",midblacksum);
			if(midblack==1)IsRoundstep=4;
	}
	//条件四补充
		if(anotherstraight==1&&midblack==0)//
	{
		midblack=1;
		midblacksum=0;
		mblacknumber=0;
		for(uint8 c=resultSet.rightBorder[jumping]-7;c<resultSet.rightBorder[jumping]+1;c++)
			for(uint8 r=jumping-3;r>2;r--)
			{
				if(image_binary[r][c]==0x00){middleblack[7+c-resultSet.rightBorder[jumping]]=r;break;}
			}
//						OLEDPrintf(1, 3, "%d %d %d %d",middleblack[0],middleblack[1],middleblack[2],middleblack[3]);
//						OLEDPrintf(1, 4, "%d %d %d %d",middleblack[4],middleblack[5],middleblack[6],middleblack[7]);
			for(uint8 i=0;i<8;i++)
			{
				if(middleblack[i]>0)
				{
					if(abs(middleblack[i]-middleblack[3])>2){midblack=0; break;}
				  midblacksum+=middleblack[i];
				}
				else {mblacknumber++;}	
			}
			if(midblack==1&&mblacknumber<3)
			{	
				midblacksum=midblacksum/(8-mblacknumber);
			if(midblacksum>7||midblacksum<3){midblack=0;}
		  }
			else midblack=0;
//				OLEDPrintf(1, 3, "%f",midblacksum);
			if(midblack==1)IsRoundstep=4;
	}
	//条件5：远处白色
	if(midblack==1)
	{
		//farwhitemid=(resultSet.leftBorder[jumping+3]+resultSet.rightBorder[jumping+3])/2;

		farwhitemid=anotherxielv1*8+resultSet.leftBorder[10]+17;
//    OLEDPrintf(1, 5, "%d  %d %d",resultSet.leftBorder[2],resultSet.rightBorder[2],farwhitemid) ;
		if(abs(farwhitemid-COL/2)<30)
		{
		for(uint8 r=1;r<10;r++)
			for(uint8 c=farwhitemid-5;c>farwhitemid-6&&c<farwhitemid+6;c++)
		  {
				farwhitesum+=(image_binary[r][c]==0x00?1:0);
			}
			if(farwhitesum<5)farwhite=1;
				//OLEDPrintf(1, 5, "%d",farwhitesum) ;
		}
			if(farwhite==1){
			    IsRoundstep=5;
				  Roundabout=1;
				  Roundaboutstep=1;
				  island_entrance=TRUE;
				
			}//OLEDPrintf(1,6, "%s","Roundabout") ;
	}
	image_binary[2][farwhitemid]=0x00;
	image_binary[3][farwhitemid]=0x00;
	image_binary[4][farwhitemid]=0x00;	
	image_binary[5][farwhitemid]=0x00;
//	OLEDPrintf(1,6, "%d  %d",farwhitesum,IsRoundstep) ;
}

bool Island_judge(){
    IsRoundabout();
	  if(island_entrance)
		{
		  return TRUE;
  	}
    else 
		{
			return FALSE;
		}		
}

bool IslandSecondEntrance_judge()
{
	if(IslandDistance>1000&&IslandDistance<4000)
	{
		IslandActionGomiddle();
	}
	
 if(IslandDistance>4000)
	{
		IslandDistance=0;
		IslandDistanceCountFlag=FALSE;
		
	}
	
	 if(IslandInflag&&IslandDistance==0)
	{	
			IslandSecondEntrancejudge();
			if(SecondEntranceInflexionFlag)
			{
				return TRUE;
			}
			else 
			{
				return FALSE ;
			}
		}
		else 
		{
			return FALSE ;
		}		
	
}

bool IslandOut_judge()
{
	if(IslandInflag&&!IslandleaveDistanceCountFlag)
	{		
			IslandOutjudge();
		    
			if(leftIslandOutflag)
			{
				 IslandOutDistanceCountFlag=TRUE ;
				 leftIslandOutflag=FALSE;
				if(IslandOutDistance>4000)//检测到出口记一段距离，这段距离内补出口线
				{
					 IslandOutDistance=0;
					 IslandleaveDistanceCountFlag=TRUE;
					 IslandOutDistanceCountFlag=FALSE ;
					 Islandleaveflag=TRUE;
					 BUZZLE_OFF; 
				}

				 return TRUE;
			}
			else
			{
				 return FALSE;
			}
	 
	}
	else if(IslandInflag&&IslandleaveDistanceCountFlag)
	{
		if(IslandSecondEntrance_judge())
		{
			IslandSecondEntranceOutProc();
			if(IslandleaveDistance>4000)//再次检测到第二个入口，记一段距离，补右边界
			{
				IslandleaveDistanceCountFlag=FALSE ;
				IslandleaveDistance=0;
				IslandInflag=FALSE;
				Islandleaveflag=FALSE;
			}	
			return TRUE;
		}	
    else
	 {
	    return FALSE;
   }		
	}
	else 
	{
			return FALSE;
	}		
}
	
void IslandActionGoright() {
    for(int16_t i = pre_sight - 3; i < pre_sight + 3; ++i) {
          resultSet.middleLine[i] = COL - 1;
    }
	  DirectionControlProc(resultSet.middleLine, COL>>1);
}
void IslandActionGomiddle()
{
	for(int16_t i = pre_sight - 3; i < pre_sight + 3; ++i) {
          resultSet.middleLine[i] = COL>>1;
    }
	  DirectionControlProc(resultSet.middleLine, COL>>1);
	
} 
/************************************************/
void IslandSecondEntrancejudge()
{
		for(uint8_t row=ROW-20;row>5;row--)
		{
			for(uint8_t col=60;col<COL-20;col++)
			{
				if(image_binary[row][col]==0x00       //中
				 &&image_binary[row-3][col]==0x00     //上
				 &&image_binary[row][col-10]==0xff		//左
				 &&image_binary[row][col+10]==0xff    //右
				 &&image_binary[row+10][col]==0xff)    //下 
				{
						SecondEntranceInflexionRow=row;
						SecondEntranceInflexionCol=col;
						SecondEntranceInflexionFlag=TRUE ;
//						BUZZLE_ON;
						OLEDPrintf(1,2, "%d  %d",SecondEntranceInflexionRow,SecondEntranceInflexionCol) ;
						break ;
				}
				
			}
		}
	
}


void IslandSecondEntranceProc()
{
	if(SecondEntranceInflexionFlag)
	{
		SecondEntranceInflexionFlag=FALSE;
		for(uint8_t row=ROW>>1;row>SecondEntranceInflexionRow;row--)
		{
			resultSet.leftBorder[row]=resultSet.leftBorder[ROW>>1]+((SecondEntranceInflexionCol-resultSet.leftBorder[ROW>>1])/(ROW/2-SecondEntranceInflexionRow))*(ROW/2-row);
			resultSet.middleLine[row]=(resultSet.leftBorder[row]+resultSet.rightBorder[row])/2;
		}
	}
}

void IslandSecondEntranceOutProc()
{
	if(SecondEntranceInflexionFlag)
	{
		SecondEntranceInflexionFlag=FALSE;
		for(uint8_t row=ROW>>1;row>SecondEntranceInflexionRow;row--)
		{
			resultSet.rightBorder[row]=resultSet.rightBorder[ROW>>1]-((resultSet.rightBorder[ROW>>1]-SecondEntranceInflexionCol)/(ROW/2-SecondEntranceInflexionRow))*(ROW/2-row);
			resultSet.middleLine[row]=(resultSet.leftBorder[row]+resultSet.rightBorder[row])/2;
		}
	}
}

/****************************************/
/****************************************/
void IslandOutjudge()
{
	OLED_Fill(0x00);  //初始清屏
	//条件一
	for(uint8_t r=ROW-25;r>1;r--)//左边界找跳变点
	{
//		if((resultSet.leftBorder[r]-resultSet.leftBorder[r-2]>20||resultSet.leftBorder[r]-resultSet.leftBorder[r-3]>20)
		if((resultSet.leftBorder[r-1]-resultSet.leftBorder[r]<0&&resultSet.leftBorder[r-2]-resultSet.leftBorder[r]<0)
			&&image_binary[r][resultSet.leftBorder[r]+5]==0xff  //跳变点右边是白
		  &&image_binary[r-4][resultSet.leftBorder[r]]==0xff  //跳变点上边是白
		  &&image_binary[r+2][resultSet.leftBorder[r]-10]==0x00 //跳变点左边是黑
		  ) 
		  {
				leftjumpout=r;
		    leftjumpoutflag=TRUE;	
				OLEDPrintf(1,1, "%d  %d",resultSet.leftBorder[r]-resultSet.leftBorder[r-2],leftjumpout) ;
				break ;				
		  }
			else 
			{
				OLEDPrintf(1,2, "%s","error STEP1") ;
			}
			
	}
	
	if(leftjumpoutflag)
	{
		leftmidtemp=resultSet.middleLine[leftjumpout+5]+40;
		for(uint8_t r=leftjumpout+5;r>1;r--)//跳变点往下X行，该行中心点往右X行开始往上搜
		{
			if(image_binary[r][leftmidtemp]==0x00   
			 &&image_binary[r-1][leftmidtemp+3]==0x00
			 &&image_binary[r-1][leftmidtemp-3]==0x00)
			{
				leftmidblackcol=leftmidtemp;
//				leftmidblackcol=COL;
				leftmidblackrow=r;
				leftmidblackflag=TRUE;
				leftIslandOutflag=TRUE;
				OLEDPrintf(1,3, "%s","Islandout ok!") ;
				BUZZLE_ON;
				break;
			}
		}
	}
	
}
/****************************************/

/****************************************/
void IslandOutProc()
{
	if(leftjumpoutflag&&leftmidblackflag)
	{
		for(uint8_t r=leftjumpout;r>leftmidblackrow;r--)
		{
			
			
			resultSet.leftBorder[r]=resultSet.leftBorder[leftjumpout]+((leftmidblackcol-resultSet.leftBorder[leftjumpout])/(leftjumpout-leftmidblackrow))*(leftjumpout-r);

//			resultSet.leftBorder[r]=resultSet.leftBorder[leftjumpout]+10*(leftjumpout-r);
//			OLEDPrintf(1,4, "%d",resultSet.leftBorder[r]) ;
			resultSet.middleLine[r]=(resultSet.leftBorder[r]+resultSet.rightBorder[r])/2;
		}
		leftjumpoutflag=FALSE;
		leftmidblackflag=FALSE;
	}
	
}

