#include <Arduino.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "spi.h"
#include "pwm.h"
#include "switch.h"
#include "I2C.h"

// MPU6050 registers
#define MPU6050_SLA 0x68
#define PWR_MGMT_1  0x6B

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

// Change these after checking Serial Monitor values
#define Y_TILT_THRESHOLD 11500
#define Z_TILT_THRESHOLD 11500

// Timer1 millisecond flag from timer.cpp
volatile unsigned char ms = 0;

// Software time counter
unsigned long systemMs = 0;
unsigned long lastSerialPrint = 0;
unsigned long lastChirpUpdate = 0;

// Accelerometer values
int16_t accelX = 0;
int16_t accelY = 0;
int16_t accelZ = 0;

// Alarm latch
bool alarmTriggered = false;

// Button event after debounce
bool buttonEvent = false;

// ---------------- Display State Machine ----------------

enum DisplayState {
  DISPLAY_SMILEY,
  DISPLAY_FROWNY
};

DisplayState displayState = DISPLAY_SMILEY;

// ---------------- Switch Debounce State Machine ----------------

enum SwitchState {
  SWITCH_WAIT_PRESS,
  SWITCH_DEBOUNCE_PRESS,
  SWITCH_WAIT_RELEASE,
  SWITCH_DEBOUNCE_RELEASE
};

volatile SwitchState switchState = SWITCH_WAIT_PRESS;
unsigned long switchTime = 0;

// ---------------- Function Prototypes ----------------

unsigned char readMPURegister(unsigned char reg);
int16_t combineBytes(unsigned char highByte, unsigned char lowByte);
void readAccelerometer();
void wakeUpMPU6050();

void updateSwitchState();
void updateDisplayState();
void updateAlarm();

// These should exist in spi.cpp
void initSPI();
void displaySmiley();
void displayFrowny();

// ---------------- ISR ----------------

ISR(INT0_vect) {
  switch (switchState) {

    case SWITCH_WAIT_PRESS:
      switchState = SWITCH_DEBOUNCE_PRESS;
      switchTime = systemMs;
      break;

    case SWITCH_WAIT_RELEASE:
      switchState = SWITCH_DEBOUNCE_RELEASE;
      switchTime = systemMs;
      break;

    default:
      break;
  }
}

// ---------------- Setup ----------------

void setup() {
  Serial.begin(9600);

  InitI2C();

  initTimer1();
  startTimer1();

  initPWM();
  setFrequency(0);

  initSwitchPD0();

  initSPI();
  displaySmiley();

  sei();

  wakeUpMPU6050();
}

// ---------------- Main Loop ----------------

void loop() {
  if (ms) {
    ms = 0;
    systemMs++;
  }

  readAccelerometer();

  updateSwitchState();

  if (buttonEvent) {
    buttonEvent = false;
    alarmTriggered = false;
    setFrequency(0);
  }

  if (abs(accelY) > Y_TILT_THRESHOLD || abs(accelZ) > Z_TILT_THRESHOLD) {
    alarmTriggered = true;
  }

  updateDisplayState();
  updateAlarm();

  if (systemMs - lastSerialPrint >= 250) {
    lastSerialPrint = systemMs;

    Serial.print("X: ");
    Serial.print(accelX);
    Serial.print("  Y: ");
    Serial.print(accelY);
    Serial.print("  Z: ");
    Serial.println(accelZ);
  }
}

// ---------------- MPU6050 ----------------

void wakeUpMPU6050() {
  StartI2C_Trans(MPU6050_SLA);
  Write(PWR_MGMT_1);
  Write(0x00);
  StopI2C_Trans();
}

unsigned char readMPURegister(unsigned char reg) {
  Read_from(MPU6050_SLA, reg);
  unsigned char data = Read_data();
  StopI2C_Trans();
  return data;
}

int16_t combineBytes(unsigned char highByte, unsigned char lowByte) {
  return (int16_t)((highByte << 8) | lowByte);
}

void readAccelerometer() {
  unsigned char xHigh = readMPURegister(ACCEL_XOUT_H);
  unsigned char xLow  = readMPURegister(ACCEL_XOUT_L);

  unsigned char yHigh = readMPURegister(ACCEL_YOUT_H);
  unsigned char yLow  = readMPURegister(ACCEL_YOUT_L);

  unsigned char zHigh = readMPURegister(ACCEL_ZOUT_H);
  unsigned char zLow  = readMPURegister(ACCEL_ZOUT_L);

  accelX = combineBytes(xHigh, xLow);
  accelY = combineBytes(yHigh, yLow);
  accelZ = combineBytes(zHigh, zLow);
}

// ---------------- Switch State Machine ----------------

void updateSwitchState() {
  switch (switchState) {

    case SWITCH_WAIT_PRESS:
      break;

    case SWITCH_DEBOUNCE_PRESS:
      if (systemMs - switchTime >= 25) {
        buttonEvent = true;
        switchState = SWITCH_WAIT_RELEASE;
      }
      break;

    case SWITCH_WAIT_RELEASE:
      break;

    case SWITCH_DEBOUNCE_RELEASE:
      if (systemMs - switchTime >= 25) {
        switchState = SWITCH_WAIT_PRESS;
      }
      break;
  }
}

// ---------------- Display State Machine ----------------

void updateDisplayState() {
  static DisplayState previousState = DISPLAY_SMILEY;

  if (alarmTriggered) {
    displayState = DISPLAY_FROWNY;
  } else {
    displayState = DISPLAY_SMILEY;
  }

  if (displayState != previousState) {
    previousState = displayState;

    if (displayState == DISPLAY_SMILEY) {
      displaySmiley();
    } else {
      displayFrowny();
    }
  }
}

// ---------------- Alarm Chirp ----------------

void updateAlarm() {
  static unsigned int frequency = 600;
  static int direction = 1;

  if (!alarmTriggered) {
    setFrequency(0);
    return;
  }

  if (systemMs - lastChirpUpdate >= 20) {
    lastChirpUpdate = systemMs;

    setFrequency(frequency);

    frequency += direction * 50;

    if (frequency >= 2500) {
      direction = -1;
    }

    if (frequency <= 600) {
      direction = 1;
    }
  }
}
