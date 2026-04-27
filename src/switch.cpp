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

void initSwitchPE5(){
    // Set PE5 as input
    DDRE &= ~(1 << DDE5);

    // Enable pull-up resistor
    PORTE |= (1 << PE5);

    // Configure INT5 (any logical change)
    EICRB &= ~(1 << ISC51);
    EICRB |= (1 << ISC50);

    // Enable INT5 interrupt
    EIMSK |= (1 << INT5);
}
