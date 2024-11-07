#pragma once 
#include <stdint.h>

void motor_init();
void update_encoder(int* position);
void update_motor(int motor_input);