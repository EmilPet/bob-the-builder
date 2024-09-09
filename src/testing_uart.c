#include "stdbool.h"
#define TX_BUF_SIZE 16

unsigned char transmit_buffer [TX_BUF_SIZE];
int buffer_read_index = 0;
int buffer_write_index = 0;

void put_buffer(unsigned char in){
	if ((buffer_write_index+1)%TX_BUF_SIZE == buffer_read_index) return; // Buffer is full, skip write (not optimal solution)
	transmit_buffer[buffer_write_index] = in;
	buffer_write_index = (buffer_write_index + 1) % TX_BUF_SIZE;
	return;
}

unsigned char get_buffer(){
	unsigned char out = transmit_buffer[buffer_read_index];
	buffer_read_index = (buffer_read_index + 1) % TX_BUF_SIZE;
	
	return out;
}

bool buffer_empty(){
	if (buffer_read_index == buffer_write_index) return true;
	else return false;
}


int main(){
    unsigned char test = 'a';
    put_buffer(test);
    unsigned char out = get_buffer();
    if (out == test) return 0;
    else return 1;
}