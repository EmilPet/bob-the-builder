#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "time.h"
#include "uart.h"
#include "can.h"
#include "servo.h"
#include "irsensor.h"
#include "motor.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile.
 * If you get somewhat cryptic errors referencing functions such as _sbrk,
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
// #include "../path_to/uart.h"

int main()
{
    uint32_t baudrate = 115200;
    uint32_t cpu_freq = 84000000;
    SystemInit();
    uart_init(cpu_freq, baudrate);

    CanInit canObj;

    // TQ is set up to be 800ns
    // Propagation delay = 6 TQ
    // Phase 1 = 5 TQ
    // Phase 2 = 2 TQs
    can_init((CanInit){.brp = 20, .phase1 = 2, .phase2 = 1, .propag = 3}, 0);

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
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

    // Uncomment after including uart above
    // uart_init(/*cpufreq*/, /*baud*/);
    // printf("Hello World\n\r");

    joystick joystick;
    slider slider;
    int ball_detected_flag = 0;
    int score = 0;
    int encoder_data = 0;
    // printf("waiting for CAN message\r\n");
    printf("\033[2J");
    printf("target initialized\r\n");
    pwm_init();
    adc_init();
    motor_init();
    uint64_t lasttime = time_now();
    uint64_t sec = 100000000;
    while (1)
    {
    
        can_decipher_msg(&joystick, &slider);
        update_encoder(&encoder_data);
        update_motor(joystick.x);
        update_servo(joystick.y);
        if(time_now() - lasttime > 1*sec){
            printf("encoder: %i\r\n", encoder_data);
            lasttime = time_now();
        }
        

    //     get_irsensor(&score, &ball_detected_flag);

    //     printf("score: %i\n\r", score);


    //     printf("\033[H");
    //     printf("joystick        \n\rx: %i   \n\ry: %i       \n\rdir: %i     \n\rslider      \n\rleft: %i        \n\rright: %i      \r\n", joystick.x, joystick.y, joystick.direction, slider.left, slider.right);
    //    // update_servo(140);
        //time_spinFor(msecs(500));
        
        //update_servo(40);
        //time_spinFor(msecs(500));

    }
}