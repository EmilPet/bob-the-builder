#ifndef STARTUP_H
#define SARTUP_H

#include <stdio.h>
#include <avr/io.h>
void port_init(){
    DDRD = 0x00;
    PORTD |= 0b1 << PD3;

}

#endif