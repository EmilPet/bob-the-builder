/*
 * Byggern.c
 *
 * Created: 9/9/2024 11:00:48 AM
 * Author : torst
 */ 

#include "uart_driver.h"
#include "sram.h"
#include "joystick_driver.h"
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	unsigned int baud_rate = 9600;
	joystick joystick;
	uint8_t slider;

	Uart_init(baud_rate);
	SRAM_init();
	while (1){
		joystick = get_joystick(joystick);
		printf("x: %i  y: %i  direction: %i \n", joystick.x, joystick.y, joystick.direction);
		//get_slider(slider);
		//printf("Slider: \n x = %i \n", slider);
		_delay_ms(100);
	}

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

