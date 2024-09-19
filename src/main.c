/*
 * Byggern.c
 *
 * Created: 9/9/2024 11:00:48 AM
 * Author : torst
 */ 

#include "uart_driver.h"
#include "sram_test.h"
#include <avr/io.h>



int main(void)
{
	unsigned int baud_rate = 9600;
	
	Uart_init(baud_rate);
	SRAM_init();
	
	/*while (1)
	{
		_delay_ms(1000);
		uint8_t data = 0xF;
		uint16_t addr = 0x0001;
		mem_write(data, addr);
		uint8_t mem_data = mem_read(addr);
		printf("%i\n", mem_data);
	}*/
	SRAM_test();
}

