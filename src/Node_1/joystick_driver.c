#include "joystick_driver.h"
#include "can.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>




int map(int value, int input_min, int input_max, int output_min, int output_max) {
	int mapped_value = (((value - input_min) * (output_max - output_min)) / (input_max - input_min)) + output_min;
	return mapped_value;
}

void adc_write ( uint8_t data , uint16_t addr ) {
	volatile char * ext_mem = ( char *) 0x1400 ; // Start address for the SRAM
	ext_mem [ addr ]= data ;
}

uint8_t adc_read ( uint16_t addr ) {
	volatile char * ext_mem = ( char *) 0x1400 ;
	uint8_t ret_val = ext_mem [ addr ];
	return ret_val ;
}

void get_adc(uint8_t* adc_input){
	uint8_t data = 0xA;
	uint16_t addr = 0x0000;
	adc_write(data, addr);
	_delay_us(2);
	// Read the voltage from adc input
	adc_input[0] = adc_read(addr);
	adc_input[1] = adc_read(addr);
	adc_input[2] = adc_read(addr);
	adc_input[3] = adc_read(addr);

}


joystick get_joystick(int previous_x, int previous_y){
	int X_MIDDLE_VALUE = 255/2 + 45; // ADC value at idle joystick
	int Y_MIDDLE_VALUE = 255/2 + 45; // ADC value at idle joystick
	
	joystick joystick;
	
	int x_summed, y_summed = 0;
	int cumsum = 50;
	for(int i = 0; i<cumsum; i++){
		uint8_t adc_input[4];
		get_adc(adc_input);
		if (adc_input[0] > Y_MIDDLE_VALUE){
			y_summed += map(adc_input[0], Y_MIDDLE_VALUE, 255, 50, 100);
		}
		else {
			y_summed += map(adc_input[0], 0, Y_MIDDLE_VALUE, 0, 50);
		}
		// split linear mapping between top and bottom part of x to make sure 50 is at the joystick resting position
		if (adc_input[1] >= X_MIDDLE_VALUE){
			x_summed += map(adc_input[1], X_MIDDLE_VALUE, 255, 50, 100);
		}
		else {
			x_summed += map(adc_input[1], 0, X_MIDDLE_VALUE, 0, 50);
		}
	}
	uint8_t y = y_summed/cumsum;
	uint8_t x = x_summed/cumsum;
	
	
	if (abs(previous_x - x) > 4){
		joystick.x = x;
	}
	else{
		joystick.x = previous_x;
	}
	
	if (abs(previous_y - y) > 4){
		joystick.y = y;
	}
	else{
		joystick.y = previous_y;
	}
	
	
	uint8_t dead_zone = 30;
	uint8_t x_0 = 50;
	uint8_t y_0 = 50;
	if( y > x && y > y_0 + dead_zone){
		joystick.direction = UP;
	}
	else if( x > y && y < y_0 - dead_zone){
		joystick.direction = DOWN;
	}
	else if( x < y && x < x_0 - dead_zone){
		joystick.direction = LEFT;
	}
	else if( x > y && x > x_0 + dead_zone){
		joystick.direction = RIGHT;
	}
	else{
		joystick.direction = NEUTRAL;
	}
	return joystick;
}


slider get_slider(){
	slider slider;
	int right = 0;
	int left = 0;
	int cumsum = 30;
	for(int i = 0; i<cumsum; i++){
		uint8_t adc_input[4];
		get_adc(adc_input);
		right += map(adc_input[2], 0, 255, 0, 100);
		left += map(adc_input[3], 0, 255, 0, 100);
	}
	//printf("left_cumulative: %i, right_cumulative: %i\n\r", left/cumsum, right/cumsum);
	slider.left = left/cumsum;
	slider.right = right/cumsum;
	return slider;
}





//TODO(ELP): add get_button function (Left = 4 Right = 5)