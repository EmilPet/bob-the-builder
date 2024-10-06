#include "spi.h"
#include <stdint.h>
//#include <avr/io.h>


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

