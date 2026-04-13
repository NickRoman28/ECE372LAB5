// LAB4 ECE372
// NICK ROMAN, AIDAN DEOGRACIAS, NOAH MONROE, ANDREW GHARTEY
// COMPLETED 4/6/26
#include <avr/io.h>
#include "pwm.h"

void initPWM3(void)
{
    // we have to edit the OCRnA registers
     // digital pin 6 to output which uses timer 4 (GOOD)
    DDRH |= (1 << DDH3);

    // Clear timer registers first using timer3
    TCCR4A = 0x00;
    TCCR4B = 0x00;


  //table 17-2, we need fast PWM, mode 15 for variable frequency (we used mode 14 for lab 4 i think)
    TCCR4A |= (1<<WGM40) | (1<<WGM41);
    TCCR4B |= (1<<WGM42) | (1<<WGM43); 
    // table 17-4, Fast PWM non-inverting mode
    TCCR4A &= ~(1<<COM4C0); 
    TCCR4A |= (1<<COM4C1);
    //^^^ sets the 0 of the 10 bit number from adc to be off and 1023 to be on 100% or 5V

    // set the prescaler equal to 1
    // this way we can use a top value of 1023 to get 15,625Hz using lecture 13 DC motor formula
    TCCR3B |= (1 << CS30);
    TCCR3B &= ~((1 << CS31) | (1 << CS32));

    //table 17-6, set prescaler to 1
    TCCR4B |= (1<<CS40);
   

    // starts at 0% duty cycle (NEED TO FIX POSSIBLY, )
    OCR4A = 0;
}

// WILL HAVE TO CONFIRM THAT THESE NUMBERS ARE RIGHT (WILL DELETE THIS MESSAGE WHEN COMPELTE)
void incFrequency(unsigned int frequency) {
OCR4A = 16000000 / frequency; //update the TOP based on the new frequency
OCR4C=0.5*OCR4A;// update the OCR4C to make sure the Duty cycle stay the same
}

//below is what we used in lab 4 for fixed freq
/* void changeDutyCycle(unsigned int adcVal)
{
    // set the value to a 10 bit range
    if (adcVal > 1023)
    {
        adcVal = 1023;
    }

    // takes the adc value and converts it into PWM duty cycle
    OCR3A = adcVal;
} */
