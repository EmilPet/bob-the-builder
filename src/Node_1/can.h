#ifndef CAN_H
#define CAN_H
#include <stdbool.h>
#include <stdint.h>

#define CAN_BUFFER_SIZE 8

typedef struct{
	uint16_t id;
	uint8_t data_length;
	uint8_t *data;
} can_message;



//void can_write(uint16_t id, uint8_t data);
void can_write(can_message frame);
bool can_read(can_message *message);

void can_message_print(can_message message);


ISR (INT0_vect);

#endif