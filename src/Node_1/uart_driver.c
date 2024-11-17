#include "uart_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>


#define set_bit(reg, bit)	(reg |= (1 << bit))
#define clear_bit(reg, bit)	(reg &= ~(1 << bit))
#define test_bit(reg, bit)	(reg & (1 << bit))




void Uart_init(unsigned int baud_rate){
	// Determine UBRR from baud_rate
	unsigned int ubrr = F_CPU/16/baud_rate-1;
	
	// Disable interrupts
	// Set baud rate in UBRR registers
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	
	// Manipulate UCSRB control register
	set_bit(UCSR0B, 7);   // Enable RX complete interrupt
	clear_bit(UCSR0B, 6); // Disable TX complete interrupt
	clear_bit(UCSR0B, 5); // Disable TX buffer available interrupt
	set_bit(UCSR0B, 4);   // Enable RX
	set_bit(UCSR0B, 3);   // Enable TX
	clear_bit(UCSR0B, 2); // Part of RX, TX character size
	clear_bit(UCSR0B, 1); // 9-bit character disable
	clear_bit(UCSR0B, 0); // 9-bit character disable

	// Manipulate UCSRC control register
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);//|(1<<UCSZ01);
	// Settings: No parity, 8-bit words, 2 stop bits,
	
	// Enable interrupts
	//sei();
	
	fdevopen(put_char, get_char);
	
	return;
}

int put_char(char data, FILE *stream){
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) ){};
	/* Put data into buffer, sends the data */
	UDR0 = data;
	if (data == '\n'){
		put_char('\r', stream);
	}
	
	return 0;
}



int* get_char(FILE *stream)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) ){
		
	};
	
	unsigned char data = UDR0;
	return (int*)data;
}
