// Author: Nicholas Roman
// Net ID: nicholasroman    
// Date: 3/23/2026
// Assignment: Lab 4
//----------------------------------------------------------------------//

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile unsigned char second;
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
    // CTC MODE
	TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
    TCCR1B &= ~(1 << WGM13);
    TCCR1B |=  (1 << WGM12);

    // sets the compare value for 1us clock with prescaler 1
    OCR1A = 62499;
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
void delaySeconds(unsigned int delay){
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






/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
    // LECTURE 7 CODE
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // sET THE BITS FOR THE prescaller
    // LECTURE 7 CODE
    // SETS THE BITS FOR 64 
    // TCCR0B|=(1 << CS00);
    // TCCR0B|=(1 << CS01);
    // TCCR0B&=~(1 << CS02);

    // set prescaler bits here to 0 since it automatically starts the timer
    TCCR0B = 0;
    OCR0A = 249;
    // clear flag
    TIFR0 |= (1 << OCF0A);  
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
   unsigned int delayCnt = 0;
    // sets the bits for the prescaler
     TCCR0B = (1 << CS01) | (1 << CS00);

     // starts the timer at 0
    TCNT0 = 0; 

    // sets the compare flab
    TIFR0 |= (1 << OCF0A); 
    while (delayCnt < delay) {
        if (TIFR0 & (1 << OCF0A)) { 
            // for timer reaching max value
            delayCnt++;
             TCNT0 = 0; 
            TIFR0 |= (1 << OCF0A);
        // restart the timer
        }
    } 
    TCCR0B = 0; 
}
