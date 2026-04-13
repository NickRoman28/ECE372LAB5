// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 3
//----------------------------------------------------------------------//

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void initTimer1();
void delaySeconds(unsigned int delay);

void startTimer1();
void stopTimer1();

void initTimer0();
void delayMs(unsigned int delay); 

extern volatile unsigned char second;

#endif
