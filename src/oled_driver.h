#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H
#define F_CPU 4915200 // Clock 
#include <avr/io.h>
#include <stdio.h>


void oled_cmd_write(uint8_t data);
void oled_data_write(uint8_t data);
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
void oled_draw_line_horisontal(int start,int end,int start_height, int width);
void oled_draw_line_vertical(int start_row,int end_row,int collum, int width);
#endif