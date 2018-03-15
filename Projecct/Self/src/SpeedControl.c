#include "SpeedControl.h"
PID leftPid, rightPid;
int16_t leftSpeed, rightSpeed;

//int16_t speed_control_sum_err_max;
//bool speed_control_on;
//int16_t speed_control_speed;
//int16_t speed_control_acc;
//int16_t speed_control_dec;
//float reduction_ratio;
//float differential_ratio;
//bool stop;
//int16_t speedAroundBarrier;
//int16_t speedInRi
uint16_t speedcontrol=90;
static int16_t SpeedControlPID(PID *pid);
static void SpeedControlFilter(int16_t newValue, PID* pid);

void SpeedControlProc(int16_t leftSpeed, int16_t rightSpeed) {
    SpeedControlFilter(leftSpeed, &leftPid);
    SpeedControlFilter(rightSpeed, &rightPid);
    motor_change(SpeedControlPID(&leftPid), SpeedControlPID(&rightPid));
}

void SpeedTargetSet(uint16_t imgProcFlag) {
//        uint16_t current_speed;
//	
//	      
//	      
////	      switch (imgProcFlag){
////				   case STRAIGHT_ROAD:current_speed=speed_control_speed+15;break;
////				   case CROSS_ROAD:   current_speed=speed_control_speed+10;break;
//// 					 case LEFTCUVER:    current_speed=speed_control_speed-8;break;
////				   case RIGHTCUVER:   current_speed=speed_control_speed-8;break;
////				   default:           current_speed=speed_control_speed ;  break;				
////		   }
////	
//				current_speed=speed_control_speed; 
//        if(directionAngle > 0) {
//					
//             current_speed -= reduction_ratio * directionAngle;
//             leftPid.targetValue = current_speed;
//             rightPid.targetValue = current_speed * (differential_ratio * directionAngle + 1);
//		
//        }  else {
//					
//             current_speed += reduction_ratio * directionAngle;
//             rightPid.targetValue = current_speed;
//             leftPid.targetValue = current_speed * (differential_ratio * (-directionAngle) + 1);
//        }
//    
		leftPid.targetValue=speedcontrol-0.3*abs(middlelineerror)-1.8*directionAngle;
		rightPid.targetValue=speedcontrol-0.3*abs(middlelineerror)+1.8*directionAngle;
 }  


int16_t SpeedControlPID(PID *pid) {
	int16_t error;
    double pValue, iValue, dValue;
    
	error = pid->targetValue - pid->currentValue;
    
    pValue = pid->kp * (error - pid->lastError);
    iValue = pid->ki * error;
    dValue = pid->kd * (error - 2 * pid->lastError + pid->prevError);
    pid->prevError = pid->lastError;
    pid->output += pValue + iValue + dValue;
    if(pid->output > 10000) {
        pid->output = 10000;
    }
    
	pid->lastError = error;
    
	return (int16_t)pid->output;
}

void SpeedControlFilter(int16_t newValue, PID* pid) {
//    uint8_t cnt;
//    pid->valueBuf[pid->cursor] = newValue;
//    pid->cursor++;
//    if(pid->cursor / SPEED_BUF_SIZE) {
//        pid->cursor = 0;
//    }
//    pid->currentValue = pid->valueBuf[0];
//    for(cnt = 1; cnt < SPEED_BUF_SIZE; cnt++) {
//        pid->currentValue += pid->valueBuf[cnt];
//        pid->currentValue /= 2;
//    }
	
	pid->currentValue=newValue;
	
}
