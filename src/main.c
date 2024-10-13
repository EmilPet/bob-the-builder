/*
 * Byggern.c
 *
 * Created: 9/9/2024 11:00:48 AM
 * Author : torst
 */
#define F_CPU 4915200 // Clock 
#include "startup.h"
#include "uart_driver.h"
#include "sram.h"
#include "joystick_driver.h"
#include <avr/io.h>
#include <util/delay.h>
#include "oled_driver.h"
#include <string.h>
#include "mcp.h"
#include "can.h"



void test_mcp(){
	// Reading and writing to CNF1 for test
	uint8_t test_data = 0xAA;
	uint8_t test_result;
	mcp_write(0x36, test_data);
//	mcp_bit_modify(0x2A, 0b11110000, 0x00);
	test_result = mcp_read(0x36);
	
	printf("Expected result:\t%i\nActual result:\t\t%i\n", test_data, test_result);	
	_delay_ms(1000);
	
}


int main(void)
{
	uint8_t fail_cond = MCUCSR;
	MCUCSR = 0;

	_delay_ms(100);
	unsigned int baud_rate = 9600;
	port_init();
	Uart_init(baud_rate);
	SRAM_init();
	oled_init();
	oled_clear();
	mcp_Init();	
	sei();
	printf("-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n");
	//printf("Fail condition: %x", fail_cond);
	
	_delay_ms(500);
	
	uint8_t data_msg[8] = {0xAA, 0xFF, 0x00, 0x11, 0xBE, 0xEF, 0x97, 0xEA};
	can_message send_frame = {.id = 0x00B, .data_length=8, .data = data_msg};
	while (1)
	{
		can_write(send_frame);
		_delay_ms(1000);
	}
	
	menu_loop();
	
}

ISR(BADISR_vect)
{

	for (;;) UDR0='!';
}