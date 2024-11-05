#include <stdint.h>
#include "irsensor.h"
#include "sam.h"
#include "time.h"

void adc_init(){

    PMC->PMC_PCER1 |= PMC_PCER1_PID37;
    //PIOB->PIO_PDR |= PIO_PDR_P20; 
    PIOB->PIO_PUDR |= PIO_PUDR_P20;
    // ADC_MR
    ADC->ADC_WPMR = ADC_WPMR_WPKEY(0x414443);
    ADC->ADC_MR = ADC_MR_TRGEN_DIS | ADC_MR_LOWRES_BITS_12 | ADC_MR_PRESCAL(1) | ADC_MR_FREERUN_ON;   
    // ADC_CHER
    ADC->ADC_CHER = ADC_CHER_CH13;
    // ADC_CR
    ADC->ADC_CR = ADC_CR_START;

    // ADC_LCDR
}

void get_irsensor(int* score){
    while(!(ADC->ADC_ISR&ADC_ISR_DRDY)){
        printf("ISR: %x\r\n", ADC->ADC_ISR);
        time_spinFor(msecs(100));
    }
    int data = ADC->ADC_LCDR;
    //int data_2 = ADC->ADC_CDR[13];
    //printf("ir sensor: %i \n\r",data);
    if(data < 1000){
        score += 1;
    }
}