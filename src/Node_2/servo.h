#pragma once
#include <stdint.h>

void pwm_init();
void update_servo(int servo_pos);
int map(int value, int input_min, int input_max, int output_min, int output_max);