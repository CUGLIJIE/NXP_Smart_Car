/*********************Ŀǰ�������Ĳ���   20180305************************************************
STEP ONE:�������ĸ�����������˵������������յ㣬�߽磬�յ��Ϸ��ڿ飬Զ����ɫ��ʶ���һ�����
STEP TWO:��ʶ�𵽵�һ����ڵ������£�ͨ���ƾ���Ͳ��ߵķ�ʽ��ʶ��ڶ������
STEP THREE:���뻷����ʶ����ڣ�ʶ�𵽳��ں��ߣ�����һ�ξ��롣
STEP FOUR:�ӻ������ڳ����Ժ��ٴ�ʶ����ұ߲��ߡ�
*/
#include "Island.h"
#include "rodeview.h"
#include "math.h"
#include "Flash_Date.h"

#define OLED_Printf_Dubug  1   //OLED������Ϣ���

#define IslandfristentranceDistanceMIN  1000
#define IslandfristentranceDistanceMAX  5500

#define IslandsecondentranceDistanceThreshold  6000
#define IslandexitDistanceThreshold   7000
#define IslandsecondentranceleaveDistanceThreshold  6000

bool f_L_Island;
bool f_R_Island;
uint8_t IslandStep=0;

uint16 IslandfristEntranceDistance;
uint16 IslandsecondEntranceDistance;
uint16 IslandexitDistance;
uint16 IslandsecondEntranceleaveDistance;

bool f_IslandfristEntranceDistance;
bool f_IslandsecondEntranceDistance;
bool f_IslandexitDistance;
bool f_IslandsecondEntranceleaveDistance;
bool f_IslandfristEntranceDistanceFinsh;

uint8_t SecondEntranceInflexionRow=0;
uint8_t	SecondEntranceInflexionCol=0;
bool		f_SecondEntranceInflexion ;

_Island_ LeftIsland;
_Island_ RightIsland;

//****************************************************************************
//  ��������Slope_Figure(uint8_t n)
//  ���ܣ�����б�ʼ���
//  ˵������
//****************************************************************************/ 
float L_Slope_Figure(uint8_t k)
{
	int slope_x=0;
	int slope_a=0;
	int slope_b=0;
	float slope=0;
	//������߽�б��   ��С���˷����ֱ��
	if(k>=4)
	{	
		slope_x=resultSet.leftBorder[k-4]+resultSet.leftBorder[k-3]+resultSet.leftBorder[k-2]+resultSet.leftBorder[k-1]+resultSet.leftBorder[k];
		slope_a=(k-4)*(resultSet.leftBorder[k-4])+(k-3)*(resultSet.leftBorder[k-3])+(k-2)*(resultSet.leftBorder[k-2])+(k-1)*(resultSet.leftBorder[k-1])+(k)*(resultSet.leftBorder[k]);
		slope_b=(k-2)*slope_x;
		slope=(slope_a-slope_b)/10.0;	
		return slope;	
	}
	else
	{
		slope=0;	
		return slope;	
	}
}

float R_Slope_Figure(uint8_t k)
{
	int slope_x=0;
	int slope_a=0;
	int slope_b=0;
	float slope=0;
	//������߽�б��
	if(k>=4)
	{	
		slope_x=resultSet.rightBorder[k-4]+resultSet.rightBorder[k-3]+resultSet.rightBorder[k-2]+resultSet.rightBorder[k-1]+resultSet.rightBorder[k];
		slope_a=(k-4)*(resultSet.rightBorder[k-4])+(k-3)*(resultSet.rightBorder[k-3])+(k-2)*(resultSet.rightBorder[k-2])+(k-1)*(resultSet.rightBorder[k-1])+(k)*(resultSet.rightBorder[k]);
		slope_b=(k-2)*slope_x;
	    slope=(slope_a-slope_b)/10.0;
		return 	slope;	
	}
	else
	{
		slope=0;	
		return slope;	
	}
}

/*****************************************************/
void IslandfristEntranceRec()
{
	for(uint8 row=20;row>4;row--)//����1���ҵ����ң��յ�
	{
		 if(resultSet.leftBorder[row-1]-resultSet.leftBorder[row] <0
			&&resultSet.leftBorder[row-2]-resultSet.leftBorder[row] <0
			&&resultSet.leftBorder[row-3]-resultSet.leftBorder[row] <0
			&&resultSet.leftBorder[row-4]-resultSet.leftBorder[row] <0
		  &&resultSet.leftBorder[row-5]-resultSet.leftBorder[row] <0
		  &&resultSet.leftBorder[row-1]!=0
		  &&resultSet.leftBorder[row-2]!=0
		  &&resultSet.leftBorder[row-3]!=0
		  &&resultSet.leftBorder[row-4]!=0
		  &&resultSet.leftBorder[row-5]!=0)//��յ�
		{
			LeftIsland.inflexion=row;
			LeftIsland.f_inflexion=TRUE;
			#if  OLED_Printf_Dubug
			OLEDPrintf(1,1, "	Linflexion  OK") ;
			#endif
			break;
		}
		else if(resultSet.rightBorder[row-1]-resultSet.rightBorder[row] >0
			    &&resultSet.rightBorder[row-2]-resultSet.rightBorder[row] >0
			    &&resultSet.rightBorder[row-3]-resultSet.rightBorder[row] >0
		      &&resultSet.rightBorder[row-4]-resultSet.rightBorder[row] >0
		      &&resultSet.rightBorder[row-5]-resultSet.rightBorder[row] >0
					&&resultSet.rightBorder[row-1]!=188
					&&resultSet.rightBorder[row-2]!=188
		      &&resultSet.rightBorder[row-3]!=188
					&&resultSet.rightBorder[row-4]!=188
					&&resultSet.rightBorder[row-5]!=188)//�ҹյ�
		{
			RightIsland.inflexion=row;
			RightIsland.f_inflexion=TRUE;
			#if OLED_Printf_Dubug
			OLEDPrintf(1,1, "Rinflexion  OK") ;
			#endif
			break;
		}
  }
	
	if(LeftIsland.f_inflexion||RightIsland.f_inflexion)//����2����һ��û������㣬�����ж���ֱ��
	{
		if(LeftIsland.f_inflexion)
		{
			  LeftIsland.f_anotherstraight=TRUE;
				for(uint8 row=20;row>2;row--)
				{
					if(resultSet.rightBorder[row-1]-resultSet.rightBorder[row] >0&&resultSet.rightBorder[row-2]-resultSet.rightBorder[row] >0)
					{
						LeftIsland.f_anotherstraight=FALSE;
						#if OLED_Printf_Dubug
						OLEDPrintf(1,2, "L_straight  Error") ;
						#endif
						break;
					}
				}
		}
		else if(RightIsland.f_inflexion)
		{
				RightIsland.f_anotherstraight=TRUE;
				for(uint8 row=20;row>2;row--)
				{
					if(resultSet.leftBorder[row-1]-resultSet.leftBorder[row] <0&&resultSet.leftBorder[row-2]-resultSet.leftBorder[row] <0)
					{
						RightIsland.f_anotherstraight=FALSE;
						#if OLED_Printf_Dubug
						OLEDPrintf(1,2, "R_straight  Error") ;
						#endif
						break;
					}
				}
		}
		
	}
	
	if(LeftIsland.f_anotherstraight||RightIsland.f_anotherstraight)//����3��������һ�ߵ�б�ʣ�ȷ����ֱ��
	{
		if(LeftIsland.f_anotherstraight)
		{
			LeftIsland.f_anotherstraight=TRUE;
			f_L_Island=TRUE;
      LeftIsland.anotherslope[0]=R_Slope_Figure(10);
//			R_Slope_Figure(5,&LeftIsland.anotherslope[0]);
//			R_Slope_Figure(6,&LeftIsland.anotherslope[0]);
		  if(fabs(LeftIsland.anotherslope[0])<1||fabs(LeftIsland.anotherslope[0])>3)
			{
				LeftIsland.f_anotherstraight=FALSE;
				f_L_Island=FALSE;
				#if OLED_Printf_Dubug
				
				OLEDPrintf(1,3, "L_Slope  Error") ;
				#endif
			}
			
		}
		else if(RightIsland.f_anotherstraight)
		{
			RightIsland.f_anotherstraight=TRUE;
			f_R_Island=TRUE;
			RightIsland.anotherslope[0]=L_Slope_Figure(10);
		  if(fabs(RightIsland.anotherslope[0])<1||fabs(RightIsland.anotherslope[0])>3)
			{
				RightIsland.f_anotherstraight=FALSE;
				f_R_Island=FALSE;
				#if OLED_Printf_Dubug
				OLEDPrintf(1,3, "R_Slope Err") ;
				#endif
			}
		}
		
	}
	if(f_L_Island||f_R_Island)//����4���ӹյ������ѣ��Ǻڿ�
	{
		if(f_L_Island)
		{
			LeftIsland.f_midblack=TRUE;
			for(uint8 col=resultSet.leftBorder[LeftIsland.inflexion]-8;col<resultSet.leftBorder[LeftIsland.inflexion];col++)
			{
				for(uint8 row=LeftIsland.inflexion-3;row>0;row--)
				{
					if(image_binary[row][col]==0x00)
					{
						LeftIsland.midblackrow[col-(resultSet.leftBorder[LeftIsland.inflexion]-8)]=row;//��¼��ÿ���ҵ��ڵ������
						break;
					}
				}
			}
			for(uint8 i=0;i<8;i++)
			{
				if(LeftIsland.midblackrow[i]>0)//�ų���һ��
				{
					if(abs(LeftIsland.midblackrow[i]-LeftIsland.midblackrow[3])>2)//�ڿ��±��ؽ���һ��ֱ��
					{
						LeftIsland.f_midblack=FALSE;
						#if OLED_Printf_Dubug						
						OLEDPrintf(1,4, "L_midblack Error") ;
						#endif
						break;
					}
				}
			}
		}
		
		else if(f_R_Island)
		{
			RightIsland.f_midblack=TRUE;
			for(uint8 col=resultSet.rightBorder[RightIsland.inflexion];col<resultSet.rightBorder[RightIsland.inflexion]+8;col++)
			{
				for(uint8 row=RightIsland.inflexion-3;row>0;row--)
				{
					if(image_binary[row][col]==0x00)
					{
						RightIsland.midblackrow[col-(resultSet.rightBorder[RightIsland.inflexion])]=row;//��¼��ÿ���ҵ��ڵ������
						break;
					}
				}
			}
			for(uint8 i=0;i<8;i++)
			{
				if(RightIsland.midblackrow[i]>0)//�ų���һ��
				{
					if(abs(RightIsland.midblackrow[i]-RightIsland.midblackrow[3])>2)//�ڿ��±��ؽ���һ��ֱ��
					{
						RightIsland.f_midblack=FALSE;
						#if OLED_Printf_Dubug
						OLEDPrintf(1,4, "R_midblack Error") ;	
						#endif
						break;
					}
				}
			}
		}	
	}
	
	if(RightIsland.f_midblack||LeftIsland.f_midblack)
	{
		if(f_L_Island&&LeftIsland.f_midblack)
		{
			LeftIsland.f_farwhite=FALSE;
			LeftIsland.farwhitestartCol=resultSet.rightBorder[10]-27;
//			LeftIsland.farwhitestartCol=resultSet.rightBorder[10]-LeftIsland.anotherslope[0]*4-17;
			if(abs(LeftIsland.farwhitestartCol-COL/2)<30)//��ͼ����������x�з�Χ��
			{
				for(uint8 row=1;row<10;row++)     //Զ��X��
				{
					for(uint8 col=LeftIsland.farwhitestartCol-5;col<LeftIsland.farwhitestartCol+5;col++)//����X��
					{
						LeftIsland.farblackpointNum+=(image_binary[row][col]==0x00?1:0);//�����������Χ�ĺڵ���  �ڵ�������һ��������Ϊ���ǰ�ɫ
					}
				}
					
				if(LeftIsland.farblackpointNum<5)
				{
					LeftIsland.f_farwhite=TRUE;
					#if OLED_Printf_Dubug
					OLEDPrintf(1, 5, "%s","L_farwhite OK!") ;
					#endif
	
				}
			}
		}
		else if(f_R_Island&&RightIsland.f_midblack)
		{
			RightIsland.f_farwhite=FALSE;
			RightIsland.farwhitestartCol=resultSet.leftBorder[10]+43;
//			RightIsland.farwhitestartCol=RightIsland.anotherslope[0]*4+resultSet.leftBorder[10]+24;
			if(abs(RightIsland.farwhitestartCol-COL/2)<30)//��ͼ����������x�з�Χ��
			{
				for(uint8 row=1;row<10;row++)     //Զ��X��
				{
					for(uint8 col=RightIsland.farwhitestartCol-5;col<RightIsland.farwhitestartCol+5;col++)//����X��
					{
						RightIsland.farblackpointNum+=(image_binary[row][col]==0x00?1:0);//�����������Χ�ĺڵ���  �ڵ�������һ��������Ϊ���ǰ�ɫ
					}
				}
					
				if(RightIsland.farblackpointNum<5)
				{
					RightIsland.f_farwhite=TRUE;
					#if OLED_Printf_Dubug
					OLEDPrintf(1, 5, "%s","R_farwhite OK!") ;
					#endif
				}
			}
		}
			
	}
	
	if(LeftIsland.f_inflexion&&LeftIsland.f_anotherstraight&&LeftIsland.f_midblack&&LeftIsland.f_farwhite)
	{
//		IslandStep++;
		IslandStep=2;
		if(IslandStep>=2)
		{
			IslandStep=3;
			LeftIsland.f_IslandIn=TRUE;//ʶ�𻷵���־
			BUZZLE_ON;
			#if OLED_Printf_Dubug
			OLEDPrintf(1, 6, "%s","L_Island success!") ;
			#endif
		}
		LeftIsland.farblackpointNum=0;
		
		LeftIsland.f_inflexion=FALSE;
		LeftIsland.f_anotherstraight=FALSE;
		LeftIsland.f_midblack=FALSE;
		LeftIsland.f_farwhite=FALSE;
	}
	else if(RightIsland.inflexion&&RightIsland.f_anotherstraight&&RightIsland.f_midblack&&RightIsland.f_farwhite)
	{
//		IslandStep++;
		IslandStep=2;
		if(IslandStep>=2)
		{
			IslandStep=3;
			RightIsland.f_IslandIn=TRUE;//ʶ�𻷵���־
			BUZZLE_ON;
			#if OLED_Printf_Dubug
			OLEDPrintf(1, 6, "%s","R_Island success!") ;
			#endif
		}
		RightIsland.farblackpointNum=0;
		
		RightIsland.f_inflexion=FALSE;
		RightIsland.f_anotherstraight=FALSE;
		RightIsland.f_midblack=FALSE;
		RightIsland.f_farwhite=FALSE;
	}
	
	LeftIsland.f_inflexion=FALSE;
	LeftIsland.f_anotherstraight=FALSE;
	LeftIsland.f_midblack=FALSE;
	LeftIsland.f_farwhite=FALSE;
	
	RightIsland.f_inflexion=FALSE;
	RightIsland.f_anotherstraight=FALSE;
	RightIsland.f_midblack=FALSE;
	RightIsland.f_farwhite=FALSE;
}

void IslandfristEntranceProc()
{
	if(LeftIsland.f_IslandIn&&f_L_Island&&IslandStep==3&&!f_IslandfristEntranceDistanceFinsh)
	{
		f_IslandfristEntranceDistance=TRUE;
		if(IslandfristEntranceDistance>IslandfristentranceDistanceMIN&&IslandfristEntranceDistance<IslandfristentranceDistanceMAX)
		{
			IslandActionGomiddle();
		}
		else if(IslandfristEntranceDistance>IslandfristentranceDistanceMAX)
		{
			BUZZLE_OFF;
			IslandfristEntranceDistance=0;
			f_IslandfristEntranceDistanceFinsh=TRUE;
			f_IslandfristEntranceDistance=FALSE;
		}	
	}
	else if(RightIsland.f_IslandIn&&f_R_Island&&IslandStep==3&&!f_IslandfristEntranceDistanceFinsh)
	{
		f_IslandfristEntranceDistance=TRUE;
		if(IslandfristEntranceDistance>IslandfristentranceDistanceMIN&&IslandfristEntranceDistance<IslandfristentranceDistanceMAX)
		{
			IslandActionGomiddle();
		}
		else if(IslandfristEntranceDistance>IslandfristentranceDistanceMAX)
		{
			IslandfristEntranceDistance=0;
			BUZZLE_OFF;
			f_IslandfristEntranceDistanceFinsh=TRUE;
			f_IslandfristEntranceDistance=FALSE;
		}	
	}
}
/*****************************************************/

/*****************************************************/
void IslandsecondEntranceRec()
{
	for(uint8_t row=ROW-30;row>2;row--)
	{
		for(uint8_t col=20;col<COL-20;col++)
		{
			if(image_binary[row][col]==0x00       //��
			 &&image_binary[row-2][col]==0x00     //��
			 &&image_binary[row][col-10]==0xff		//��
			 &&image_binary[row][col+10]==0xff    //��
			 &&image_binary[row+10][col]==0xff)    //�� 
			{
					SecondEntranceInflexionRow=row;
					SecondEntranceInflexionCol=col;
					f_SecondEntranceInflexion=TRUE ;
					break ;
			}
			
		}
	}
}

void IslandsecondEntranceProc()
{
	if((f_SecondEntranceInflexion&&f_L_Island&&IslandStep==3&&f_IslandfristEntranceDistanceFinsh)||(f_SecondEntranceInflexion&&f_R_Island&&IslandStep==5))
	{
		f_SecondEntranceInflexion=FALSE;
		for(uint8_t row=pre_sight+3;row>pre_sight-3;row--)
		{
			resultSet.rightBorder[row]=resultSet.rightBorder[ROW>>1]-((resultSet.rightBorder[ROW>>1]-SecondEntranceInflexionCol)/(ROW/2-SecondEntranceInflexionRow))*(pre_sight+3-row);
			resultSet.middleLine[row]=(resultSet.leftBorder[row]+resultSet.rightBorder[row])/2;
		}
	}
	else if((f_SecondEntranceInflexion&&f_R_Island&&IslandStep==3&&f_IslandfristEntranceDistanceFinsh)||(f_SecondEntranceInflexion&&f_L_Island&&IslandStep==5))
	{
		f_SecondEntranceInflexion=FALSE;
		for(uint8_t row=pre_sight+3;row>pre_sight-3;row--)
		{
			resultSet.leftBorder[row]=resultSet.leftBorder[ROW>>1]+((SecondEntranceInflexionCol-resultSet.leftBorder[ROW>>1])/(ROW/2-SecondEntranceInflexionRow))*(pre_sight+3-row);
			resultSet.middleLine[row]=(resultSet.leftBorder[row]+resultSet.rightBorder[row])/2;
		}
	}
}
/*****************************************************/


/*****************************************************/
void IslandExitRec()
{
	if(f_L_Island)
	{
		for(uint8_t row=ROW-25;row>4;row--)//��߽��������
		{
			if((resultSet.rightBorder[row-1]-resultSet.rightBorder[row]>0&&resultSet.rightBorder[row-2]-resultSet.rightBorder[row]>0)
				&&image_binary[row+2][resultSet.rightBorder[row]+5]==0x00  //������ұ��Ǻ�
				&&image_binary[row-4][resultSet.rightBorder[row]]==0xff  //������ϱ��ǰ�
				&&image_binary[row][resultSet.rightBorder[row]-10]==0xff //���������ǰ�
				) 
				{
					LeftIsland.exitinflexion=row;
					LeftIsland.f_exitinflexion=TRUE;
					#if OLED_Printf_Dubug					
					OLEDPrintf(70, 7, "%d",LeftIsland.exitinflexion) ;
					#endif
					break ;				
				}		
		}
	
		if(LeftIsland.f_exitinflexion)
		{
			LeftIsland.f_exitinflexion=FALSE;
			LeftIsland.exitMidTemp=resultSet.middleLine[LeftIsland.exitinflexion+5]-70;
			if(LeftIsland.exitMidTemp>COL){LeftIsland.exitMidTemp=COL;}
			for(uint8_t row=LeftIsland.exitinflexion+5;row>1;row--)//���������X�У��������ĵ�����X�п�ʼ������
			{
				if(image_binary[row][LeftIsland.exitMidTemp]==0x00   
				 &&image_binary[row-1][LeftIsland.exitMidTemp+3]==0x00
				 &&image_binary[row-1][LeftIsland.exitMidTemp-3]==0x00)
				{
					LeftIsland.exitMidblackcol=LeftIsland.exitMidTemp;
					LeftIsland.exitMidblackrow=row;
					LeftIsland.f_Exit=TRUE;
					BUZZLE_ON;
					#if OLED_Printf_Dubug
					OLEDPrintf(70, 7, "%s","80") ;
					#endif				
					break;
				}
			}
		}
	}
	else if(f_R_Island)
	{
		for(uint8_t row=ROW-25;row>4;row--)//��߽��������
		{
			if((resultSet.leftBorder[row-1]-resultSet.leftBorder[row]<0&&resultSet.leftBorder[row-2]-resultSet.leftBorder[row]<0)
				&&image_binary[row][resultSet.leftBorder[row]+5]==0xff  //������ұ��ǰ�
				&&image_binary[row-4][resultSet.leftBorder[row]]==0xff  //������ϱ��ǰ�
				&&image_binary[row+2][resultSet.leftBorder[row]-10]==0x00 //���������Ǻ�
				) 
				{
					RightIsland.exitinflexion=row;
					RightIsland.f_exitinflexion=TRUE;
					#if OLED_Printf_Dubug
          OLEDPrintf(70, 7, "%d","RightIsland.exitinflexion") ;	
					#endif					
					break ;				
				}		
		}
	
		if(RightIsland.f_exitinflexion)
		{
			RightIsland.f_exitinflexion=FALSE;
			RightIsland.exitMidTemp=resultSet.middleLine[RightIsland.exitinflexion+5]+70;
			if(RightIsland.exitMidTemp>COL){RightIsland.exitMidTemp=COL;}
			for(uint8_t row=RightIsland.exitinflexion+5;row>3;row--)//���������X�У��������ĵ�����X�п�ʼ������
			{
				if(image_binary[row][RightIsland.exitMidTemp]==0x00   
				 &&image_binary[row-1][RightIsland.exitMidTemp+3]==0x00
				 &&image_binary[row-1][RightIsland.exitMidTemp-3]==0x00)
				{
					RightIsland.exitMidblackcol=RightIsland.exitMidTemp;
					RightIsland.exitMidblackrow=row;
					RightIsland.f_Exit=TRUE;
					BUZZLE_ON;
					#if OLED_Printf_Dubug
					 OLEDPrintf(70, 7, "%s","80") ;	
					#endif
					break;
				}
			}
		}
	}
	
}

void IslandExitProc()
{
	if(LeftIsland.f_Exit&&f_L_Island&&IslandStep==4)
	{
			LeftIsland.f_Exit=FALSE;
		for(uint8_t row=pre_sight+3;row>pre_sight-3;row--)
		{
			resultSet.rightBorder[row]=resultSet.rightBorder[LeftIsland.exitinflexion]-((resultSet.rightBorder[LeftIsland.exitinflexion]-LeftIsland.exitMidblackcol)/(LeftIsland.exitinflexion-LeftIsland.exitMidblackrow))*(LeftIsland.exitinflexion-row);
			resultSet.middleLine[row]=(resultSet.leftBorder[row]+resultSet.rightBorder[row])/2;
		}
	
	}
	else if(RightIsland.f_Exit&&f_R_Island&&IslandStep==4)
	{
		RightIsland.f_Exit=FALSE;
		for(uint8_t row=pre_sight+3;row>pre_sight-3;row--)
		{
			resultSet.leftBorder[row]=resultSet.leftBorder[RightIsland.exitinflexion]+((RightIsland.exitMidblackcol-resultSet.leftBorder[RightIsland.exitinflexion])/(RightIsland.exitinflexion-RightIsland.exitMidblackrow))*(RightIsland.exitinflexion-row);
			resultSet.middleLine[row]=(resultSet.leftBorder[row]+resultSet.rightBorder[row])/2;
			#if OLED_Printf_Dubug
			OLEDPrintf(100, 2, "%s","60") ;		
			#endif
		}
		
	}
}
/*****************************************************/
/*****************************************************/
bool IslandfristEntrancejudge()
{
	IslandfristEntranceRec();
	if((LeftIsland.f_IslandIn&&f_L_Island)||(RightIsland.f_IslandIn&&f_R_Island))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool IslandsecondEntrancejudge()
{
	IslandfristEntranceProc();
	if(f_IslandfristEntranceDistanceFinsh||IslandStep==5)
	{
		IslandsecondEntranceRec();
		if(IslandsecondEntranceDistance>IslandsecondentranceDistanceThreshold&&IslandStep==3)
		{
			BUZZLE_OFF;
			IslandsecondEntranceDistance=0;
			f_IslandfristEntranceDistanceFinsh=FALSE;
			f_IslandsecondEntranceDistance=FALSE;
			IslandStep=4; 
			
		}
		else if(IslandsecondEntranceleaveDistance>IslandsecondentranceleaveDistanceThreshold&&IslandStep==5)
		{
			BUZZLE_OFF;
			IslandsecondEntranceleaveDistance=0;
			f_IslandsecondEntranceleaveDistance=FALSE;
			IslandStep=0;
			
			f_R_Island=FALSE;
			f_L_Island=FALSE;
			RightIsland.f_IslandIn=FALSE;
			LeftIsland.f_IslandIn=FALSE;
		}
	}
	if(f_SecondEntranceInflexion)
	{
		if(IslandStep==3)f_IslandsecondEntranceDistance=TRUE;       //��һ�μ�⵽��ʼ�ƾ���  
		else if(IslandStep==5)f_IslandsecondEntranceleaveDistance=TRUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool IslandExitjudge()
{
	IslandExitRec();
	if(IslandexitDistance>IslandexitDistanceThreshold)
	{
		BUZZLE_OFF;
		IslandexitDistance=0;
		f_IslandexitDistance=FALSE;
		IslandStep=5;
	}
	if(LeftIsland.f_Exit||RightIsland.f_Exit)
	{
		f_IslandexitDistance=TRUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}
/*****************************************************/

void IslandActionGomiddle()
{
	if(f_L_Island)
	{
		for(int16_t i = pre_sight - 3; i < pre_sight + 3; ++i) 
		{
						resultSet.rightBorder[i]-= i*1.5;
			      resultSet.middleLine[i]=resultSet.rightBorder[i];
		}
	}
	else if(f_R_Island)
	{
		for(int16_t i = pre_sight - 3; i < pre_sight + 3; ++i) 
		{
						resultSet.leftBorder[i]+= i*1.5;
			      resultSet.middleLine[i]=resultSet.leftBorder[i];
		}
	}
	
	  DirectionControlProc(resultSet.middleLine, COL>>1);
	
} 


