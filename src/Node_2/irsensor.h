#pragma once 
#include <stdint.h>

void adc_init();
void get_irsensor(int* score, int* ball_detected_flag);