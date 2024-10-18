#ifndef SPI_H
#define SPI_H
#include <stdint.h>

void SPI_Init(void);
void SPI_Transmit(char data);
uint8_t SPI_Receive();
#endif