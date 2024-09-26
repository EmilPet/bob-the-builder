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
#include "oled_driver.h"


int main(void)
{
	unsigned int baud_rate = 9600;
	/*
	joystick joystick;
	slider slider;
	*/
	char word[] = "read if gay";

	Uart_init(baud_rate);
	SRAM_init();
	oled_init();
	oled_clear();
	
	oled_pos(1, 10);
	oled_write(word);
	oled_pos(3, 10);
	oled_write("or stopid");
	oled_pos(5, 10);
	oled_write("COLLUM");
	oled_pos(6, 10);
	oled_write("COLUMB");

	/*
	while (1)
	{
		joystick = get_joystick(joystick);
		slider = get_slider(slider);
			printf("------------------------------------------------------\njoystick(x: %i  y: %i  direction: %i) \nslider(left: %i  right: %i) \n-------------------------------------------------------\n",
			joystick.x, joystick.y, joystick.direction, slider.left, slider.right);
		// get_slider(slider);
		// printf("Slider: \n x = %i \n", slider);
		_delay_ms(500);
	}

	while (1)
	{
		_delay_ms(1000);
		uint8_t data = 0xF;
		uint16_t addr = 0x0001;
		mem_write(data, addr);
		uint8_t mem_data = mem_read(addr);
		printf("%i\n", mem_data);
	}
	SRAM_test();
	*/
}
