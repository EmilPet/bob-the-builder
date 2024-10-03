#include "joystick_driver.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>




int map(int value, int input_min, int input_max, int output_min, int output_max) {
    int mapped_value = (value - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
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



joystick get_joystick(){
    joystick joystick;
    uint8_t adc_input[4];
    get_adc(adc_input);
    uint8_t y = map(adc_input[0], 0, 255, 0, 100); 
    uint8_t x = map(adc_input[1], 0, 255, 0, 100);
    joystick.x = x;
    joystick.y = y;
    
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

slider get_slider(slider slider){
    uint8_t adc_input[4];
    get_adc(adc_input);
    uint8_t left = map(adc_input[2], 0, 255, 0, 100);
    uint8_t right = map(adc_input[3], 0, 255, 0, 100);
    slider.left = left;
    slider.right = right;
    return slider;
}

//TODO(ELP): add get_button function (Left = 4 Right = 5)