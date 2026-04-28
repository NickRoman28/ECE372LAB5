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

void buzzerOn(unsigned int f) {
    unsigned long ocr;
    if (f ==0) {
        buzzerOff();
        return;
    }

    ocr = (16000000UL/ (2UL * 8UL * f)) -1;
    if (ocr > 65535UL) {
    ocr = 65535UL;
    }

    OCR4A = (unsigned int)ocr;

    

void incFrequency() {
   unsigned int f;
    buzzerEnable();
    for (f = 1000; f <= 3000; f+=100) {
        buzzerOn(f);
        delayMs(50);
}

void decFrequency() {
    unsigned int f;
    buzzerEnable();
    for (f = 3000; f>= 1000; f-=100) {
        buzzerOn(f);
        delayMs(50);
    }
}


