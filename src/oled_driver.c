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

int oled_goto_columb(int columb){
	int row_MSB = columb / 16;
	int row_LSB = columb % 16;

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
    oled_goto_columb(0);
    for (int row = 0; row < 8; row++){
        for (int columb = 0; columb < 128; columb++){
            oled_goto_row(row);
            oled_data_write(0b00000000);
			}
    }
}


void oled_write_char(char asci_index){
    for(int i; i<8; i++){
        oled_data_write(pgm_read_byte(&font8[asci_index - 32][i]));
    }
}

void oled_write_center_text(char* input_string){
    int center = (127 - strlen(input_string)*8)/2;
    oled_goto_columb(center);
    for(int i=0; i<strlen(input_string); i++){
        oled_write_char(input_string[i]);
    }
}
void oled_write(char* input_string){
    for(int i=0; i<strlen(input_string); i++){
        oled_write_char(input_string[i]);
    }
}
void oled_pos(int row, int collum){
    oled_goto_row(row);
    oled_goto_columb(collum);
}