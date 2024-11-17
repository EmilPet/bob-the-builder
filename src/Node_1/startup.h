#ifndef STARTUP_H
#define SARTUP_H

#include <stdio.h>
#include <avr/io.h>
void port_init(){
	// D3 - Joystick Button
	// D2 - CAN interrupt
	// B2 - LED active low
	
	// D3 as pull up input
	DDRD = 0x00;
	PORTD |= 0b1 << PD3;
	
	// B2 as output high
	DDRB |= (1 << PB2);
	PORTB |= (1 << PB2);
	
	// Enable falling edge interrupt on D2
	MCUCR = 0x02;
	GICR |= 0x40;
	
}

#endif