#include "sam.h"
#include "servo.h"
#include <stdio.h>
enum PWM_CONTROL {
    DISABLE_ALL_WRITE_PROTECC = 0b11111100,
    CPRD_20MS_CONSTANT = 1640
};

int map(int value, int input_min, int input_max, int output_min, int output_max) {
    int mapped_value = (value - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
    return mapped_value;
}

// 
void pwm_init(){
    uint8_t prescaler = 1;
    PWM->PWM_WPCR = PWM_WPCR_WPKEY(0x50574D) | DISABLE_ALL_WRITE_PROTECC;
    // Enable peripheral clock
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    //PIOB->PIO_ODR |= PIO_ODR_P13;
    PIOB->PIO_PDR |= PIO_PDR_P12 | PIO_PDR_P13;
    PIOB->PIO_ABSR |= PIO_ABSR_P12 | PIO_ABSR_P13;
    // Disable PWM
    PWM->PWM_DIS = PWM_DIS_CHID0 | PWM_DIS_CHID1;
    // Sett PWM clock
    PWM->PWM_CLK = PWM_CLK_DIVA(1) | PWM_CLK_PREA(0);
    // Sett master clock div
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024 | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_2 | PWM_CMR_CPOL;
    // Sett periode
    PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_CPRD(CPRD_20MS_CONSTANT);
    PWM->PWM_CH_NUM[0].PWM_CPRD = PWM_CPRD_CPRD(CPRD_20MS_CONSTANT);

    // Sett duty cycle
    PWM->PWM_CH_NUM[1].PWM_CDTY = PWM_CDTY_CDTY(CPRD_20MS_CONSTANT*10/100);
    PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_CDTY_CDTY(CPRD_20MS_CONSTANT*10/100);

    // Enable PWM
    PWM->PWM_ENA = PWM_ENA_CHID0 | PWM_ENA_CHID1;
}

void update_servo(int servo_pos){
    int duty_cycle_percentage = map(servo_pos,0, 100, 500,1000);
    if(duty_cycle_percentage < 500){
        duty_cycle_percentage = 500;
        //printf("ERROR: DUTYCYCLE BELOW THRESHOLD VALUE\r\n");
    }
    else if (duty_cycle_percentage>1000)
    {
        duty_cycle_percentage = 1000;
        //printf("ERroR: DUTYCYCLE ABOVE THRESHOLD VALUE\r\n");
    }
    PWM->PWM_CH_NUM[1].PWM_CDTY = PWM_CDTYUPD_CDTYUPD(CPRD_20MS_CONSTANT*duty_cycle_percentage/10000);
}