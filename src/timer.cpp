// Author: Nicholas Roman
// Net ID: nicholasroman    
// Date: 3/23/2026
// Assignment: Lab 4
//----------------------------------------------------------------------//

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile unsigned char ms;
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
    // CTC MODE
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
    TCCR1B &= ~(1 << WGM13);
    TCCR1B |=  (1 << WGM12);

    // sets the compare value for 1us clock with prescaler 1
    OCR1A = 249;
    // start with the timer being off 
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

	//FOLLOWING IS NEW________________________________________________

	TIMSK1 |= (1 << OCIE1A);

	//Clears compare flag just in case
    TIFR1 |= (1 << OCF1A); 

//	___________________________________________________________________
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayms(unsigned int delay){
    // start timer with prescaler 1
    TCCR1B = (TCCR1B & ~((1 << CS12) | (1 << CS11) | (1 << CS10))) | (1 << CS12);

    while (delay > 0) {
        // reset counter
        TCNT1 = 0;              
        TIFR1 |= (1 << OCF1A);  

        while ((TIFR1 & (1 << OCF1A)) == 0) {
            // wait for compare match
        }

        delay--;
    }

    // stop timer
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

//FOLLOWING IS NEW_______________________________________________________________

void startTimer1() {
    TCNT1 = 0;
    TIFR1 |= (1 << OCF1A);

    // Prescaler = 256
    TCCR1B &= ~((1 << CS11) | (1 << CS10));
    TCCR1B |= (1 << CS12);
}

void stopTimer1() {
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

ISR(TIMER1_COMPA_vect) {
    second = 1;
}

//__________________________________________________________________________________________






