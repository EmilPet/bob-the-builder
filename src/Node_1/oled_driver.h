#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H
#define F_CPU 4915200 // Clock
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>

enum SUB_MENUS{
	MAIN_MENU		= 0,
	PLAY_MENU		= 1,
	SETTINGS_MENU	= 2,
	CONTROLS_MENU	= 3
	};
	
enum GAME_STATE{
	MENU_TIME = 0,
	GAME_TIME = 1
	};

void oled_cmd_write(uint8_t data);
void oled_data_write(const uint8_t data);
void oled_init();
void oled_write();
int oled_goto_column(int column);
int oled_goto_row(int row);
void oled_clear();
void oled_write_char(char asci_index);
void oled_write_center_text(char* input_string);
void oled_write(char* input_string);
void oled_pos(int row, int collum);
void oled_write_center_text_inverted(char* input_string, int border);
void update_menu(int current_menu, int joystick_input);
void menu_loop();

void start_scoreboard();
void update_game_display(uint8_t score);

bool button_pressed();
#endif