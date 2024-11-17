#ifndef SRAM_H
#define SRAM_H

#include <stdlib.h>
#include <stdint.h>
#include <avr/iom162.h>

void SRAM_init(void);

void mem_write(uint8_t data , uint16_t addr);
uint8_t mem_read( uint16_t addr );
void SRAM_test(void);

#endif