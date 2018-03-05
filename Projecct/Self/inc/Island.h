#ifndef __ISLAND_H
#define __ISLAND_H

#include "common.h"

void IsRoundabout(void);
bool Island_judge(void);
void IslandActionGoright(void);
extern int32_t IslandDistance;
extern int32_t IslandOutDistance;
 extern bool island_entrance;
extern  bool IslandDistanceCountFlag ;
extern  bool IslandOutDistanceCountFlag ;
 void IslandActionGomiddle(void);
 void IslandOutProc(void);
 void IslandOutjudge(void);
 bool IslandOut_judge(void);
 
void IslandSecondEntrancejudge(void);
void IslandSecondEntranceProc(void);
bool IslandSecondEntrance_judge(void);
#endif


