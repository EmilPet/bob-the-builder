#ifndef MCP_H
#define MCP_H

enum MCP_COMMANDS {
    MCP_RESET =    0b11000000,
    MCP_READ =     0b00000110,
    MCP_READ_RX0 = 0b10010000, // bit 1-2 determine(0-indexed) 
    MCP_READ_RX1 = 0b10010010,
    MCP_WRITE =    0b00000010,
    MCP_LOAD_TX0 = 0b01000000, // 3 final bits determine
    MCP_LOAD_TX1 = 0b01000001,
    MCP_LOAD_TX2 = 0b01000010,
    MCP_RTS_TX0 =  0b10000001, // 3 final bits determine
    MCP_RTS_TX1 =  0b10000010,
    MCP_RTS_TX2 =  0b10000100,
    MCP_READ_STATUS = 0b10100000,
    MCP_RX_STATUS= 0b10110000,
    MCP_BITMOD =   0b00000101
};

#endif