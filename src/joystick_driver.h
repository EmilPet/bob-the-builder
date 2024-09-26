#ifndef JOYSTICK_DRIVER_H
#define JOYSTICK_DRIVER_H

#include <avr/io.h>
#include <stdio.h>

typedef struct
{
    uint8_t x;
    uint8_t y;
    enum Direction {UP, DOWN, LEFT, RIGHT, NEUTRAL};
    enum Direction direction;
}joystick;

typedef struct
{
    uint8_t left;
    uint8_t right;
} slider;


void adc_write ( uint8_t data , uint16_t addr );
uint8_t adc_read ( uint16_t addr );
void get_adc(uint8_t* adc_input); 
joystick get_joystick(joystick joystick);
slider get_slider(slider slider);




#endif