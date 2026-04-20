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
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
