#include <avr/io.h>
#include <avr/iom162.h>
#define set_bit ( reg , bit ) ( reg |= ( 1 << bit ) )
int main(void){
	DDA0 = 1; // A0 output
	
	set_bit(PORTA, 0);
	
	while (1){
		
	}
}