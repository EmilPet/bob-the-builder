#include <stdint.h>
#include "motor.h"
#include "sam.h"
#include "time.h"
#include "servo.h"


// ENCODER
// 36.6.14.4 Position and Rotation Measurement
// 36.6.14.3 Direction Status and Change Detection
// 36.6.14.2 Input Pre-processing
// TC_CV TC_BMR TC_CMR TC_CCR



void motor_init(){
    // Disable PIO PC25 PC26
    PIOC->PIO_PDR |= PIO_PDR_P25 | PIO_PDR_P26;
    // Enable DIR pin
    PIOC->PIO_PER |= PIO_PER_P23;
    PIOC->PIO_PUDR |= PIO_PUDR_P23;
    PIOC->PIO_OER |= PIO_OER_P23;
    // Enable timer counter TC2
    PMC->PMC_PCER1 |= PMC_PCER1_PID33;
    // Enable per
    PIOC->PIO_ABSR |= PIO_ABSR_P25 | PIO_ABSR_P26; 
    // Enable Quadratic decoding and position mode
    TC2->TC_BMR |= TC_BMR_QDEN | TC_BMR_POSEN;
    // Set XC0 as input clock (må cleare wave bit?)
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0 | TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG;
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}
int update_encoder(){
    return map(TC2->TC_CHANNEL[0].TC_CV, 0, 2828, 0, 100);
}

void update_motor(int motor_input){
    int duty_cycle_percentage;

    if(motor_input > 2){
        duty_cycle_percentage = map(motor_input, 0, 100, 400, 2000); 
        PIOC->PIO_CODR = PIO_SODR_P23;   
    }
    else if (motor_input < -2)
    {
        duty_cycle_percentage = map(motor_input, 0, -100, 400, 2000);    
        PIOC->PIO_SODR = PIO_CODR_P23;
    }
    else{
        duty_cycle_percentage = 0;
    }
    

    if(duty_cycle_percentage < 0){
        duty_cycle_percentage = 0;
        //printf("ERROR: DUTYCYCLE BELOW THRESHOLD VALUE\r\n");
    }
    else if (duty_cycle_percentage>1000)
    {
        duty_cycle_percentage = 1000;
        //printf("ERroR: DUTYCYCLE ABOVE THRESHOLD VALUE\r\n");
    }
    PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_CDTYUPD_CDTYUPD(duty_cycle_percentage);
}
void calib_encoder(){
    update_motor(-50);
    time_spinFor(msecs(500));
    update_motor(0);
    time_spinFor(msecs(100));
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;  // Reset TC counter for TC2 Channel 0
}

int pid_output(controller *pid){
    float e = pid->r - pid->y;
    pid->integralterm += pid->Ki * e * pid->dt;
    pid->u = pid->Kp*e + pid->integralterm;
    return pid->u;
}