#pragma once 
#include <stdint.h>

void adc_init();
int get_irsensor();
void get_score(int* score, int* ball_detected_flag);
