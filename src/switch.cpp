// LAB4 ECE372
// NICK ROMAN, AIDAN DEOGRACIAS, NOAH MONROE, ANDREW GHARTEY
// COMPLETED 4/6/26
// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>
/*
 * Initializes pull-up resistor on PD0 and sets it into input mode
 */

void initSwitchPD0(){
    //initialize as input using and with a 0
    DDRD &= ~(1<<PD0);
    //initialize the pullup resistor
    PORTD |= (1 << PD0);
    //initialize/enable external interrupts for PD0 (INT0)
    EICRA &= ~(1 << ISC01);
    EICRA |= (1 << ISC00);
    EIMSK |= (1<<INT0);
}
