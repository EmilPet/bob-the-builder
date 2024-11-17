#ifndef CAN_H
#define CAN_H
#include "joystick_driver.h"
#include <stdbool.h>
#include <stdint.h>

#define CAN_BUFFER_SIZE 8

enum CAN_ID {
	JOYSTICK_ID		= 0x001,
	SLIDER_ID		= 0x002,
	MENU_ID			= 0x003,
	GAME_SCORE_ID	= 0x004,
	BTN_ID			= 0x005
};

typedef struct{
	uint16_t id;
	uint8_t data_length;
	uint8_t *data;
} can_message;



//void can_write(uint16_t id, uint8_t data);
void can_write(can_message frame);
bool can_read(can_message *message);

void can_message_print(can_message message);

void can_i_send_joystick(joystick data);
void can_i_send_slider(slider data);
int can_i_get_game_score();
void can_i_start_game(void);
void can_i_stop_game(void);
void can_i_send_btn_press();

ISR (INT0_vect);

#endif