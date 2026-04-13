// LAB4 ECE372
// NICK ROMAN, AIDAN DEOGRACIAS, NOAH MONROE, ANDREW GHARTEY
// COMPLETED 4/6/26
#include <avr/io.h>
#include "pwm.h"

void initPWM3(void)
{
    // we have to edit the OCRnA registers
     // digital pin 5
    DDRE |= (1 << DDE3);

    // Clear timer registers first using timer3
    TCCR3A = 0x00;
    TCCR3B = 0x00;

    // table 17-4, Fast PWM non-inverting mode
    // what this does is sets the 0 of the 10 bit number from adc to be off and 1023 to be on 100% or 5V
    TCCR3A |= (1 << COM3A1);
    TCCR3A &= ~(1 << COM3A0);

    // fast PWM 10 bit mode for fixed frequency table 17-2 14
    TCCR3A |= (1 << WGM30) | (1 << WGM31);
    TCCR3B |= (1 << WGM32);
    TCCR3B &= ~(1 << WGM33);

    // set the prescaler equal to 1
    // this way we can use a top value of 1023 to get 15,625Hz using lecture 13 DC motor formula
    TCCR3B |= (1 << CS30);
    TCCR3B &= ~((1 << CS31) | (1 << CS32));

    // starts at 0% duty cycle 
    OCR3A = 0;
}

void changeDutyCycle(unsigned int adcVal)
{
    // set the value to a 10 bit range
    if (adcVal > 1023)
    {
        adcVal = 1023;
    }

    // takes the adc value and converts it into PWM duty cycle
    OCR3A = adcVal;
}
