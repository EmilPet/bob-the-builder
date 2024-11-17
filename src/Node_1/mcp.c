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
	_delay_ms(1000);
	uint8_t status_reg = mcp_read(CANSTAT);
	printf("Status_reg: %x\n", status_reg);
	printf("Status_reg_masked: %x\n", (status_reg & CAN_MODE_MASK));
	if ((status_reg & CAN_MODE_MASK) != CAN_MODE_CONFIG) // If not in CONFIG
	{
		printf("MCP2515 is not in config mode after reset.\n");
		mcp_write(CANCTRL, CAN_MODE_CONFIG);
		
		status_reg = mcp_read(CANSTAT);
		printf("Status_reg: %x\n", status_reg);
	}
	
	//	Interrupt Setup, Interrupt on message recieved in rx buffer 0
	mcp_write(CANINTE, 0x01);
	
	// Set up data rate
	// Permanently murderous!
 	mcp_write(CNF1, 0b00000001);
 	mcp_write(CNF2, 0b10010011);
 	mcp_write(CNF3, 0b00000001);
// 	mcp_write(CNF2, 0b00010011);	
//	mcp_write(CNF1, 0b00000001);
//	printf("CNF1: %x, CNF2: %x\n", mcp_read(CNF1), mcp_read(CNF2));
	
	//	Enable Loopback mode
	mcp_write(CANCTRL, 0x00);
	
	
	
	//mcp_check_tx_regs();
	
// 	printf("MCP setup finished\n");
// 	status_reg = mcp_read(CANSTAT);
// 	
 	printf("Status_reg: %x\n", status_reg);
//	printf("Error codes: %x\n", mcp_read(EFLG));
	
	
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











// Filters, just in case





// 	// 	 Filter Setup, accept only ID 0x0F messages on filter 0
// 	uint16_t filter0 = 0x0B;
// 	uint8_t filter0_lsb = filter0 % 8; // bit 3-0
// 	uint8_t filter0_msb = (filter0 >> 3) % 256; // bit 10-4
// 	mcp_write(RXF0SIDH, filter0_msb);
// 	mcp_write(RXF0SIDL, filter0_lsb << 5);
//
// 	uint16_t filter1 = 0x3FF;
// 	uint8_t filter1_lsb = filter1 % 8; // bit 3-0
// 	uint8_t filter1_msb = (filter1 >> 3) % 256; // bit 10-4
// 	mcp_write(RXF1SIDH, filter1_msb);
// 	mcp_write(RXF1SIDL, filter1_lsb << 5);
//
// 	uint16_t mask0 = 0x3FF; // Full mask
// 	uint8_t mask0_lsb = mask0 % 8; // bit 3-0
// 	uint8_t mask0_msb = (mask0 >> 3) % 256; // bit 10-4
// 	mcp_write(RXM0SIDH, mask0_msb);
// 	mcp_write(RXM0SIDL, mask0_lsb << 5);
//
// 	// More filter setup, Block absolutely everything to Buffer 1
// 	// ID 0x3FF should not be used
//
// 	uint16_t mask1 = 0x3FF; // Full mask
// 	uint8_t mask1_lsb = mask1 % 8; // bit 3-0
// 	uint8_t mask1_msb = (mask1 >> 3) % 256; // bit 10-4
// 	mcp_write(RXM1SIDH, mask1_msb);
// 	mcp_write(RXM1SIDL, mask1_lsb << 5);
//
// 	uint16_t filter_block = 0x3FF;
// 	uint8_t filter_block_lsb = filter_block % 8; // bit 3-0
// 	uint8_t filter_block_msb = (filter_block >> 3) % 256; // bit 10-4
// 	mcp_write(RXF2SIDH, filter_block_msb);
// 	mcp_write(RXF2SIDL, filter_block_lsb << 5);
// 	mcp_write(RXF3SIDH, filter_block_msb);
// 	mcp_write(RXF3SIDL, filter_block_lsb << 5);
// 	mcp_write(RXF4SIDH, filter_block_msb);
// 	mcp_write(RXF4SIDL, filter_block_lsb << 5);
// 	mcp_write(RXF5SIDH, filter_block_msb);
// 	mcp_write(RXF5SIDL, filter_block_lsb << 5);