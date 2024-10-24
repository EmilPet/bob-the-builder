#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "time.h"
#include "uart.h"
#include "can.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

int main()
{
    uint32_t baudrate = 9600;
    uint32_t cpu_freq = 84000000;
    SystemInit();
    uart_init(cpu_freq, baudrate);
    
    CanInit canObj;
    
    // TQ is set up to be 800ns
    // Propagation delay = 6 TQ
    // Phase 1 = 5 TQ
    // Phase 2 = 2 TQs
    can_init((CanInit){.brp = 20, .phase1 = 2, .phase2 = 1, .propag = 3}, 0);
    
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    // Set clock
    PMC->PMC_PCER0 |= (1 << ID_PIOB);

    // Set pin as output
    PIOB->PIO_PER = PIO_PB13;
    PIOB->PIO_OER = PIO_PB13;
/*  PIOB->PIO_IFDR = PIO_PB13;
    PIOB->PIO_IDR = PIO_PB13;
    PIOB->PIO_MDDR = PIO_PB13;
    PIOB->PIO_PUDR = PIO_PB13;
    PIOB->PIO_OWDR = PIO_PB13; */
 
    //Uncomment after including uart above
    //uart_init(/*cpufreq*/, /*baud*/);
    //printf("Hello World\n\r");

    CanMsg can_msg;
    printf("waiting for CAN message\r\n");
    while (1)
    {
        // todo sug en feit en -EP
        uint8_t success = can_rx(&can_msg);
        if (success){
            can_printmsg(can_msg);
            time_spinFor(msecs(1000));
        }
    }  
}