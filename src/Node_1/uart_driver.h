#ifndef UART_DRIVER_H
#define UART_DRIVER_H

// Usage:
//    - Initiate with Uart_init
//    - Write to remote with Print_UART
//    - Deal with input from remote in ISR

#define F_CPU 4915200 // Clock

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void USART_Init( unsigned int ubrr );
void Uart_init(unsigned int baud_rate);
int put_char(char data, FILE *stream);
int* get_char(FILE *stream);

#endif