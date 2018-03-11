#include "SteerActuator.h"


bool steer_actuator_on;
int16_t steer_actuator_left;
int16_t steer_actuator_middle=730;
int16_t steer_actuator_right;

void SteerActuatorInit() {
    ftm_pwm_init(STEER_ACTUATOR_PORT,STEER_ACTUATOR_CHL,50,steer_actuator_middle); 
}

void SteerActuatorReset() {
    SteerActuatorOut(steer_actuator_middle);
}

void SteerActuatorOut(int16_t out) {
    ftm_pwm_duty(STEER_ACTUATOR_PORT, STEER_ACTUATOR_CHL, out);
}
