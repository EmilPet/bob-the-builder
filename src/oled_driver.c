#include "oled_driver.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include "font.h"

void oled_cmd_write(uint8_t data)
{
    uint16_t addr = 0x0000;
    volatile char *ext_mem = (char *)0x1000; // Start address for the oled command
    ext_mem[addr] = data;
}

void oled_data_write(uint8_t data)
{
    uint16_t addr = 0x0000;
    volatile char *ext_mem = (char *)0x1200; // Start address for the oled data
    ext_mem[addr] = data;
}

void oled_init()
{
    oled_cmd_write(0xae); // display off
    oled_cmd_write(0xa1); // segment remap
    oled_cmd_write(0xda); // common pads hardware: alternative
    oled_cmd_write(0x12);
    oled_cmd_write(0xc8); // common output scan direction:com63~com0
    oled_cmd_write(0xa8); // multiplex ration mode:63
    oled_cmd_write(0x3f);
    oled_cmd_write(0xd5); // display divide ratio/osc. freq. mode
    oled_cmd_write(0x80);
    oled_cmd_write(0x81); // contrast control
    oled_cmd_write(0x50);
    oled_cmd_write(0xd9); // set pre-charge period
    oled_cmd_write(0x21);
    oled_cmd_write(0x20); // Set Memory Addressing Mode
    oled_cmd_write(0x02);
    oled_cmd_write(0xdb); // VCOM deselect level mode
    oled_cmd_write(0x30);
    oled_cmd_write(0xad); // master configuration
    oled_cmd_write(0x00);
    oled_cmd_write(0xa4); // out follows RAM content
    oled_cmd_write(0xa6); // set normal display
    oled_cmd_write(0xaf); // display on

    oled_cmd_write(0x20);
    oled_cmd_write(0b10); // Page mode for scroll
}

int oled_goto_column(int column){
	int row_MSB = column / 16;
	int row_LSB = column % 16;

	oled_cmd_write(0x00 + row_LSB);  //velge en rad fra 0-127 med binærtall. 4 LSB her og 4 msb under
	oled_cmd_write(0x10 + row_MSB);
}

int oled_goto_row(int row){
    if (row > 7){
        row = 7;
    }
    oled_cmd_write(0xB0 + row); // row is from 0-7
}

void oled_clear(){
    oled_goto_column(0);
    for (int row = 0; row < 8; row++){
        for (int column = 0; column < 128; column++){
            oled_goto_row(row);
            oled_data_write(0b00000000);
			}
    }
}


void oled_write_char(char asci_index){
    for(int i = 0; i<8; i++){
        oled_data_write(pgm_read_byte(&font8[asci_index - 32][i]));
    }
}
void oled_write_char_invert(char asci_index){
    for(int i = 0; i<8; i++){
        int data = pgm_read_byte(&font8[asci_index - 32][i]);
        int inverted = ~data;
        oled_data_write(inverted);
    }
}


void oled_write_center_text(char* input_string){
    int center = (127 - strlen(input_string)*8)/2;
    oled_goto_column(center);
    for(int i=0; i<strlen(input_string); i++){
        oled_write_char(input_string[i]);
    }
}
void oled_write_center_text_inverted(char* input_string, int border){
    int center = (127 - strlen(input_string)*8)/2;
    oled_goto_column(center - border);
    for (int i = 0; i < border; i++){
        oled_data_write(0xFF);
    }
    for(int i=0; i<strlen(input_string); i++){
        oled_write_char_invert(input_string[i]);
    }
    for (int i = 0; i < border; i++){
        oled_data_write(0xFF);
    }
}


void oled_write(char* input_string){
    for(int i=0; i<strlen(input_string); i++){
        oled_write_char(input_string[i]);
    }
}
void oled_pos(int row, int collum){
    oled_goto_row(row);
    oled_goto_column(collum);
}

void oled_draw_line_horisontal(int start,int end,int start_height, int width) {
    int length = end-start;
    
    int row = start_height / 8; // Position in the 8-pixel row  
    uint8_t row_pixel = ((1 << width) - 1) << (7 - start_height % 7);
    
    oled_pos(row, start);
    for (int i = 0; i <= length; i++){
        oled_data_write(row_pixel);
    }  
}
void oled_draw_line_vertical(int start_row,int end_row,int collum, int width) {
    int length = end_row - start_row;
    
    for (int i = 0; i <= length; i++){
        oled_pos(start_row + i, collum);
        for (int i = 0; i < width; i++){
            oled_data_write(0b11111111);
        }
    }  
}

void update_menu(int current_menu, int joystick_input){
	oled_clear();
	oled_goto_row(1);

	const char **selected_menu = NULL;
	
	switch (current_menu){
		case 0: // Main Menu
			oled_write_center_text("MAIN MENU");
			selected_menu = (const char *[]){"Settings", "Play", "Quit"};
			break;

		case 1: // Play Menu
			oled_write_center_text("PLAY MENU");
			selected_menu = (const char *[]){"*<]:^D", "Coming Soon", "Return"};
			break;

		case 2: // Settings Menu
			oled_write_center_text("SETTINGS MENU");
			selected_menu = (const char *[]){"UwU", "OwO", "Return"};
			break;
	}

	for (int i = 0; i < 3; i++)
	{
		oled_goto_row(i + 2);
		int current_item = (i + joystick_input) % 3;
		if (i == 1)
		{
			oled_write_center_text_inverted(selected_menu[current_item], 10);
		}
		else
		{
			oled_write_center_text(selected_menu[current_item]);
		}
	}

	
	_delay_ms(700);
}