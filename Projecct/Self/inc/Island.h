#ifndef __ISLAND_H
#define __ISLAND_H

#include "common.h"

extern bool f_L_Island;
extern bool f_R_Island;
extern uint8_t IslandStep;

extern uint16 IslandfristEntranceDistance;
extern uint16 IslandsecondEntranceDistance;
extern uint16 IslandexitDistance;
extern uint16 IslandsecondEntranceleaveDistance;

extern bool f_IslandfristEntranceDistance;
extern bool f_IslandsecondEntranceDistance;
extern bool f_IslandexitDistance;
extern bool f_IslandsecondEntranceleaveDistance;

extern uint8_t SecondEntranceInflexionRow;
extern uint8_t	SecondEntranceInflexionCol;
extern bool		f_SecondEntranceInflexion ;

typedef struct{
	//First entrance variable
	uint8_t inflexion;
	uint8_t midblackrow[8];
	uint8_t midblackNum;
	float anotherslope[3];
	bool  f_inflexion;
	bool  f_anotherstraight;
	bool  f_midblack;
	uint8_t farwhitestartCol;
	uint8_t farblackpointNum;
	bool  f_farwhite;
	bool  f_IslandIn;
	
	//Exit variable
	uint8_t exitinflexion;
	uint8_t exitMidTemp;
	uint8_t exitMidblackrow;
	uint8_t exitMidblackcol;
	bool    f_exitinflexion;
	bool    f_Exit;
	
}_Island_;

void IslandfristEntranceRec(void);
void IslandfristEntranceProc(void);
void IslandsecondEntranceRec(void);
void IslandsecondEntranceProc(void);
void IslandExitRec(void);
void IslandExitProc(void);
bool IslandfristEntrancejudge(void);
bool IslandsecondEntrancejudge(void);
bool IslandExitjudge(void);
void IslandActionGomiddle(void);
float L_Slope_Figure(uint8_t k);
float R_Slope_Figure(uint8_t k);
	
#endif


