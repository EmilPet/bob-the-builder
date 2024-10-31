#include "sam.h"
#include "servo.h"
#include <stdio.h>
// Dont work :(
void pwm_init(){
    uint8_t prescaler = 1;
    PIOA->PIO_OER = PIO_OER_P21;
    PMC->PMC_PCER0 = PMC_PCER0_PID21;
    PWM->PWM_DIS = PWM_DIS_CHID1;
    // Sett master clock div
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_MCK;
    // Sett duty cycle
    PWM->PWM_CH_NUM[1].PWM_CDTY = PWM_CDTY_CDTY(10);
    // Sett periode (?)
    PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_CPRD(50);
    // Sett frekvens
    PWM->PWM_ENA = PWM_ENA_CHID1; // Finn riktig channel
    //REG_PWM_WPCR = WPCMD;  
}
