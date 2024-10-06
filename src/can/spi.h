#ifndef SPI_H
#define SPI_H
#include <stdint.h>


void SPI_MasterInit(void);
void SPI_Transmit(uint8_t data);
uint8_t SPI_Receive(void);

#endif