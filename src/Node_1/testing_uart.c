/*
 * Byggern.c
 *
 * Created: 9/9/2024 11:00:48 AM
 * Author : torst
 */ 

#include "uart_driver.h"
#include <avr/io.h>



int main(void)
{
	unsigned int baud_rate = 9600;
	
	Uart_init(baud_rate);
    while (1) 
    {
		char word[8];
		scanf("%c", &word);
		printf(word);
    }
}

