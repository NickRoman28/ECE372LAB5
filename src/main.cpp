#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "spi.h"
#include "pwm.h"
#include "switch.h"
#include "I2Ch"
// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:

      //plays sound on buzzer, NEED TO MAKE SURE THIS FREUQNECY RANGE IS RIGHT
      for (unsigned int f = 500; f <= 3000; f += 50)
    {
        setFrequency(f);
        delayMs(20);
    }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
