#include "spi.h"
#include <stdint.h>
#include "mcp.h"
#include <avr/io.h>
#include <util/delay.h>


// Read content at address
uint8_t mcp_read(uint8_t address){
	// Set the slave select pin low
	PORTB &= ~(1 << PB4);
	// Send the read command
	SPI_Transmit(MCP_READ);
	// Send the address
	SPI_Transmit(address);
	// Read the data
	uint8_t data = SPI_Receive();
	// Set the slave select pin high
	PORTB |= (1 << PB4);
	return data;
}


void mcp_write(uint8_t address, uint8_t data){
	// Set the slave select pin low
	PORTB &= ~(1 << PB4);
	// Send the write command
	SPI_Transmit(MCP_WRITE);
	// Send the address
	SPI_Transmit(address);
	// Send the data
	SPI_Transmit(data);
	// Set the slave select pin high
	PORTB |= (1 << PB4);
}

void mcp_request_to_send(uint8_t buffer_number){
	// Set the slave select pin low
	PORTB &= ~(1 << PB4);
	
	switch (buffer_number)
	{
	case 0:
		SPI_Transmit(MCP_RTS_TX0);
		break;
	case 1:
		SPI_Transmit(MCP_RTS_TX1);
		break;
	case 2:
		SPI_Transmit(MCP_RTS_TX2);
		break;
	default:
		SPI_Transmit(MCP_RTS_ALL);
	}
	
	// Set the slave select pin high
	PORTB |= (1 << PB4);
}

uint8_t mcp_read_status(){
	// Set the slave select pin low
	PORTB &= ~(1 << PB4);
	
	SPI_Transmit(MCP_READ_STATUS);
	
	uint8_t status = SPI_Receive();
	
	// Set the slave select pin high
	PORTB |= (1 << PB4);
	
	return status;
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	// Set the slave select pin low
	PORTB &= ~(1 << PB4);
	
	SPI_Transmit(MCP_BITMOD);
	SPI_Transmit(address);
	SPI_Transmit(mask);
	SPI_Transmit(data);
	
	// Set the slave select pin high
	PORTB |= (1 << PB4);
}

void mcp_reset(){
	// Set the slave select pin low
	PORTB &= ~(1 << PB4);
	
	SPI_Transmit(MCP_RESET);
	
	// Set the slave select pin high
	PORTB |= (1 << PB4);
}


void mcp_Init(){
	SPI_Init();
	_delay_ms(500);
	mcp_reset();
 	uint8_t status_reg = mcp_read(CANSTAT);
	 printf("Status_reg: %i\n", status_reg);
 	if ((status_reg & 0b11100000) != 0b10000000) // If not in CONFIG
 	{
 		printf("MCP2515 is not in config mode after reset.\n");
 		mcp_write(CANCTRL, 0b10000000);
		 
		status_reg = mcp_read(CANSTAT);
		printf("Status_reg: %i\n", status_reg);
 	}
	
	// 	 Filter Setup, accept only ID 0x0F messages on filter 0
	uint16_t filter0 = 0x0F;
	uint8_t filter0_lsb = filter0 % 8; // bit 3-0
	uint8_t filter0_msb = (filter0 >> 3) % 256; // bit 10-4
 	mcp_write(RXF0SIDH, filter0_msb);
 	mcp_write(RXF0SIDL, filter0_lsb << 5);
	 
	uint16_t mask0 = 0x3FF; // Full mask
	uint8_t mask0_lsb = mask0 % 8; // bit 3-0
	uint8_t mask0_msb = (mask0 >> 3) % 256; // bit 10-4
 	mcp_write(RXM0SIDH, mask0_msb);
 	mcp_write(RXM0SIDL, mask0_lsb << 5);
 	
 	//	Interrupt Setup, Interrupt on message recieved in rx buffer 0
 	mcp_write(CANINTE, 0x01);
 	
	//	Enable Loopback mode
 	mcp_write(CANCTRL, 0b01010000);
	
	//mcp_check_tx_regs();
	
	printf("MCP setup finished\n");
	status_reg = mcp_read(CANSTAT);
		
	printf("Status_reg: %i\n", status_reg);
	 
	
}


// Testing specific test case storage, VERY VULNERABLE TO BEING OUTDATED
void mcp_check_tx_regs(){
	printf("-------------TX regs-------------\n");
	
	uint8_t reg_content;
	reg_content = mcp_read(TXB0SIDL);
	printf("TXB0SIDL:\t%i\n", reg_content);
	reg_content = mcp_read(TXB0SIDH);
	printf("TXB0SIDH:\t%i\n", reg_content);
	
	
	// Assign Data length (static 1 byte WIP)
	reg_content = mcp_read(TXB0DLC);
	printf("TXB0DLC:\t%i\n", reg_content);
	
	// Assign Data
	reg_content = mcp_read(TXB0D0);
	printf("TXB0D0:\t\t%i\n", reg_content);
	
	reg_content = mcp_read(CANINTE);
	printf("CANINTE:\t%i\n", reg_content);
	
	printf("---------------------------------\n");
	
}
