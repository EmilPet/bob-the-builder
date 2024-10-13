#include "can.h"
#include "mcp.h"
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void can_write(can_message frame){
	uint8_t id_lsb = frame.id % 8; // bit 3-0
	uint8_t id_msb = (frame.id >> 3) % 256; // bit 10-4
	//printf("%x, %x\n", id_msb, id_lsb);
	// Wait for free tx buffer
	while(mcp_read(TXB0CTRL) & 0b1000 == 0b1000){
	}
	
	// Assign ID
	mcp_write(TXB0SIDL, (id_lsb << 5));
	mcp_write(TXB0SIDH, id_msb);
	
	// Assign Data length
	
	mcp_write(TXB0DLC, frame.data_length);
	
	// Write data registers
	for (int i = 0; i < frame.data_length; i++){
		//printf("%x\n", frame.data[i]);
		mcp_write(TXB0D0 + i, frame.data[i]);
	}
	
	// Send TX Buffer 0
	mcp_bit_modify(TXB0CTRL, 0b1000, 0b1000);

}


void can_message_print(can_message message){
	printf("---CAN_MESSAGE---\n");
	printf("ID:\t%i\n", message.id);
	printf("Data length:\t%i\n", message.data_length);
	for(int i = 0; i < message.data_length; i++){
		printf("%i\n", message.data[i]);
	}
	printf("-----------------\n");
}

ISR (INT0_vect){
	uint16_t msg_id = (mcp_read(RXB0SIDH) << 3);
	msg_id |= ((mcp_read(RXB0SIDL) & 0b11100000) >> 5);
	printf("msg_id:\t%x\n", msg_id);
	
	uint8_t data_size = (mcp_read(RXB0DLC) & 0xF);
	printf("data_size:\t%x\n", data_size);
	uint8_t data[8];
	for (int i = 0; i < data_size; i++){
		data[i] = mcp_read(RXB0D0 + i);
		printf("data[%i]:\t%x\n", i, data[i]);
	}
	
  	mcp_bit_modify(CANINTF, 0x01, 0x00);
	return;
}