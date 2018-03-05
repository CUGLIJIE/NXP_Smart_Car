#include "DirectionControl.h"
int16_t pre_sight=20;
bool direction_control_on;
float directionAngle;
int16_t middlelineerror;
//float direction_control_kd =0.2;
//float direction_control_kpj = 0.01;
//float direction_control_kpc = 0.001; //0.0001
float direction_control_kd = 0.2;
float direction_control_kpj = 0.02;
float direction_control_kpc = 0.000125; //0.0001
static int16_t DirectionErrorGet(int16_t* middleLine, int16_t expectMiddle);
static int16_t DirectionControlPID(int16_t dirError);


void DirectionControlProc(int16_t* middleLine, int16_t expectMiddle) 
{

	middlelineerror=DirectionErrorGet(middleLine, expectMiddle);
  SteerActuatorOut(DirectionControlPID(middlelineerror));
}

int16_t DirectionErrorGet(int16_t* middleLine, int16_t expectMiddle) {
    float avgMiddle = 0;
    for(int16_t i = pre_sight - 3; i < pre_sight + 3; ++i) {
        avgMiddle += middleLine[i];
    }
    avgMiddle /= 6;
		searchForBordersStartIndex_lastimg=avgMiddle;
//	  OLEDPrintf(1, 5, "%f",avgMiddle) ;
    return 2.2*(expectMiddle - avgMiddle);
}


//int16_t DirectionControlPID(int16_t error) {
// 	static int16_t lastError = 0;
//    
//	   directionAngle = Min_f(direction_control_kpj* error + (error * error) * direction_control_kpc, 0.6) //0.28
//        * error + direction_control_kd * (error - lastError);
//    
//    directionAngle = Limit_f(directionAngle, -13, 14);
//    
//    lastError = error;
//    motor_change(4000-40*abs(error)-50*directionAngle,4000-40*abs(error)+50*directionAngle);
//    return directionAngle * 5.556 + 750;
//}
int16_t DirectionControlPID(int16_t error) {
 	static int16_t lastError = 0;
    
	   directionAngle = Min_f(direction_control_kpj + (error * error) * direction_control_kpc, 0.2) //0.28
        * error + direction_control_kd * (error - lastError);
    
    directionAngle = Limit_f(directionAngle, -13, 14);
    
    lastError = error;
//    motor_change(4000-10*abs(error)-60*directionAngle,4000-10*abs(error)+60*directionAngle);
    return directionAngle * 5.556 + 790;
}
