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

const char *main_menu[] = {"Play", "Settings", "Quit"};
const char *play_menu[] = {"Coming", "Soon", "Return"};
const char *settings_menu[] = {"^  ^", "  __  ", "Return"};

int joystick_input = 200 * 3;

bool button_pressed()
{
	uint8_t d_inputs = PIND;
	if (d_inputs & 0b1000)
	{
		return false;
	}
	return true;
}


int main(void)
{
	joystick joystick;
	unsigned int baud_rate = 9600;
	port_init();
	Uart_init(baud_rate);
	SRAM_init();
	oled_init();
	oled_clear();
	bool update_display = true;
	int selected_menuitem = 4;// Starting with invalid
	int current_menu = 0; // Starting in main


	update_menu(current_menu, joystick_input);
	oled_draw_line_horisontal(0,50,10,1);
	oled_draw_line_horisontal(0,5,10,1);


	while (0)
	{
		joystick = get_joystick();

		if (joystick.direction == UP)
		{
			joystick_input -= 1;
			update_menu(current_menu, joystick_input);
		}
		else if (joystick.direction == DOWN)
		{
			joystick_input += 1;
			update_menu(current_menu, joystick_input);
		}

		if (button_pressed())
		{
			while (button_pressed()){}
			selected_menuitem = (joystick_input + 1) % 3;
			if(current_menu == 0){
				if(selected_menuitem == 0){
					current_menu = 1;
				}
				if(selected_menuitem == 1){
					current_menu = 2;
				}
			}
			if(current_menu == 1){
				if(selected_menuitem == 2){
					current_menu = 0;
				}
			}
			if(current_menu == 2){
				if(selected_menuitem == 2){
					current_menu = 0;
				}
			}
			

			update_menu(current_menu, joystick_input);
		}
	}
}
