#pragma once 
#include <stdint.h>

typedef struct 
{
    float Kp;
    float Ki;
    int r;
    int u;
    int y;
    float integralterm;
    float dt;
} controller;


void calib_encoder();
void motor_init();
int update_encoder();
void update_motor(int motor_input);
int pid_output(controller *pid);
void activate_solenoid();