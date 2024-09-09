#include <avr/io.h>
#include <avr/iom162.h>
#include <avr/interrupt.h>
#include "stdbool.h"

#define FOSC 1843200 // Clock 
#define TX_BUF_SIZE 16


1 #define set_bit ( reg , bit ) ( reg |= ( 1 << bit ) )
2 #define clear_bit ( reg , bit ) ( reg &= ˜ ( 1 << bit ) )
3 #define test_bit ( reg , bit ) ( reg & ( 1 << bit ) )

bool tx_reg_free;
unsigned char transmit_buffer [TX_BUF_SIZE];
int buffer_read_index = 0;
int buffer_write_index = 0;

void Print_UART (unsigned char[] text){
	int text_length = *(&text + 1) - text;
	for (int i = 0; i < text_length; i++){
		uart_buffer_transmit(text[i]);
	}
}

void put_buffer(unsigned char in){
	if ((buffer_write_index+1)%TX_BUF_SIZE == buffer_read_index) return; // Buffer is full, skip write (not optimal solution)
	transmit_buffer[buffer_write_index] = in;
	buffer_write_index = (buffer_write_index + 1) % TX_BUF_SIZE;
	return;
}

unsigned char get_buffer(){
	if (buffer_read_index == buffer_write_index) return NULL; // Empty buffer
	
	unsigned char out = transmit_buffer[buffer_read_index];
	buffer_read_index = (buffer_read_index + 1) % TX_BUF_SIZE;
	
	return out;
}

bool buffer_empty(){
	if (buffer_read_index == buffer_write_index) return true;
	else return false;
}


void uart_init(int baud_rate){
	// Determine UBRR from baud_rate
	unsigned int ubrr = FOSC/16/baud_rate-1
	
	// Disable interrupts
	cli();
	// Set baud rate in UBRR registers
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	
	// Manipulate UCSRB control register
	set_bit(UCSR0B, 7);   // Enable RX complete interrupt
	clear_bit(UCSR0B, 6); // Disable TX complete interrupt
	set_bit(UCSR0B, 5);   // Enable TX buffer available interrupt
	set_bit(UCSR0B, 4);   // Enable RX
	set_bit(UCSR0B, 3);   // Enable TX 
	clear_bit(UCSR0B, 2); // Part of RX, TX character size
	clear_bit(UCSR0B, 1); // 9-bit character disable
	clear_bit(UCSR0B, 0); // 9-bit character disable

	// Manipulate UCSRC control register
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
	// Settings: No parity, 8-bit words, 2 stop bits, 
	
	// Enable interrupts
	sei();
	return;
}


void uart_buffer_transmit(unsigned char data){
	cli();
	if (tx_reg_free){
		tx_reg_free = false;
		UDR0 = data; // Put data in TX register
	}
	else{ // If not free, put in the queue
		put_buffer(data);
	}sei();
	return;

}

// Interrupt Service Routine when tx register is ready
ISR(USART0_UDRE_vect){
	if (buffer_empty()){ // There isn't data in the buffer
		tx_reg_free = true;
	}
	else{
		UDR0 = get_buffer();
	}
	return;
}

// Interrupt service routine when rx register is ready
ISR(USART0_RXC_vect){ // Exercise 1 implementation, only echo back.
	unsigned char recieved = UDR0;
	uart_buffer_transmit(recieved);
	return;
}