#include <avr/io.h>


void SPI_Init(void){
	printf("SPI init\n");
	/* Set MOSI, SCK and SS output, all others input */
	DDRB |= (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_Transmit(uint8_t data){
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

uint8_t SPI_Receive(void){
	/* Start transmission */
	SPDR = 0x00;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}