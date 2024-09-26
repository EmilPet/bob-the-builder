#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H
#include <avr/io.h>
#include <stdio.h>


void oled_cmd_write(uint8_t data);
void oled_data_write(uint8_t data);
void oled_init();
void oled_write();
int oled_goto_columb(int columb);
int oled_goto_row(int row);
void oled_clear();
void oled_write_char(char asci_index);
void oled_write_center_text(char* input_string);
void oled_write(char* input_string);
void oled_pos(int row, int collum);
#endif