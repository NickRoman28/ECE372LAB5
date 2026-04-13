// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 3
//----------------------------------------------------------------------//

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void initTimer1();
void delayms(unsigned int delay);

void startTimer1();
void stopTimer1();


extern volatile unsigned char ms;

#endif
