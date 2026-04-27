// LAB5 ECE372
// NICK ROMAN, AIDAN DEOGRACIAS, NOAH MONROE, ANDREW GHARTEY
// COMPLETED 4/6/26
#include <avr/io.h>
#include "pwm.h"

void initPWM(void)
{
    // OC4C is on PH5 (pin 8) - corrected from DDH3
    DDRH |= (1 << DDH5);

    // Clear timer 4 registers 
    TCCR4A = 0x00;
    TCCR4B = 0x00;

    // Fast PWM mode 15 (WGM43:40 = 1111), TOP = OCR4A
    TCCR4A |= (1 << WGM40) | (1 << WGM41);
    TCCR4B |= (1 << WGM42) | (1 << WGM43); 

    // Non-inverting mode on channel C (OC4C)
    TCCR4A &= ~(1 << COM4C0); 
    TCCR4A |= (1 << COM4C1);

    // Prescaler = 1
    TCCR4B |= (1 << CS40);

    // Start silent
    OCR4A = 0;
    OCR4C = 0;
}

void incFrequency(unsigned int frequency) {
    if (frequency == 0) { 
        OCR4A = 0; 
        OCR4C = 0; 
        return; 
    }
    OCR4A = 16000000UL / frequency;
    OCR4C = OCR4A / 2;  // 50% duty cycle
}

void setFrequency(unsigned int frequency)
{
    if (frequency == 0)
    {
        OCR4A = 0;
        OCR4C = 0;
        return;
    }

    // f = 16MHz / (N * (1 + TOP))  where N = 1
    unsigned long top = (16000000UL / frequency) - 1;

    OCR4A = (unsigned int)top;  // sets frequency via TOP
    OCR4C = OCR4A / 2;          // 50% duty cycle
}