#ifndef MCP_H
#define MCP_H
#include <stdint.h>


enum MCP_COMMANDS {
	MCP_RESET =    0xC0,
	MCP_READ =     0b00000011,
	MCP_READ_RX0 = 0b10010000, // bit 1-2 determine(0-indexed)
	MCP_READ_RX1 = 0b10010010,
	MCP_WRITE =    0b00000010,
	MCP_LOAD_TX0 = 0b01000000, // 3 final bits determine
	MCP_LOAD_TX1 = 0b01000001,
	MCP_LOAD_TX2 = 0b01000010,
	MCP_RTS_TX0 =  0b10000001, // 3 final bits determine
	MCP_RTS_TX1 =  0b10000010,
	MCP_RTS_TX2 =  0b10000100,
	MCP_RTS_ALL =  0b10000111,
	MCP_READ_STATUS = 0b10100000,
	MCP_RX_STATUS= 0b10110000,
	MCP_BITMOD =   0b00000101
};

enum MCP_REGS {
	TXB0CTRL	= 0x30,// TX Buffers.
	TXB0SIDH	= 0x31,
	TXB0SIDL	= 0x32,
	TXB0DLC		= 0x35,
	TXB0D0		= 0x36,
	//osv
	
	// Other Control
	CANINTE		= 0x2B,
	CANINTF		= 0x2C,
	CANSTAT		= 0x0E,
	CANCTRL		= 0xFF,
	CNF1		= 0x2A,
	CNF2		= 0x29,
	CNF3		= 0x28,
	EFLG		= 0x2D,
		
	// RX Buffers
	RXB0CTRL	= 0x60,
	RXB0SIDH	= 0x61,
	RXB0SIDL	= 0x62,
	RXB0DLC		= 0x65,
	RXB0D0		= 0x66,
	
	RXB1CTRL	= 0x70,
	RXB1SIDH	= 0x71,
	RXB1SIDL	= 0x72,
	RXB1DLC		= 0x75,
	RXB1D0		= 0x76,
	
	// Filters
	RXF0SIDH	= 0x00,
	RXF0SIDL	= 0x01,
	RXF1SIDH	= 0x04,
	RXF1SIDL	= 0x05,
	RXF2SIDH	= 0x08,
	RXF2SIDL	= 0x09,
	RXF3SIDH	= 0x10,
	RXF3SIDL	= 0x11,
	RXF4SIDH	= 0x14,
	RXF4SIDL	= 0x15,
	RXF5SIDH	= 0x18,
	RXF5SIDL	= 0x19,
	
	// Masks
	RXM0SIDH	= 0x20,
	RXM0SIDL	= 0x21,
	RXM1SIDH	= 0x24,
	RXM1SIDL	= 0x25
	
};

enum div {
	CAN_MODE_MASK	= 0xE0,
	CAN_MODE_CONFIG = 0x80
	};

uint8_t mcp_read(uint8_t address);
void mcp_write(uint8_t address, uint8_t data);
void mcp_request_to_send(uint8_t buffer_number);
uint8_t mcp_read_status();
void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void mcp_reset();

void mcp_Init();
void mcp_check_tx_regs();


#endif