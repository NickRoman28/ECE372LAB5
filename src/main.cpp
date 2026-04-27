#include <Arduino.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "spi.h"
#include "pwm.h"
#include "switch.h"
#include "I2C.h"

// MPU6050 info
#define MPU6050_SLA 0x68
#define PWR_MGMT_1  0x6B

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

// Adjust these after testing actual Serial readings
#define Y_TILT_THRESHOLD 11500
#define Z_TILT_THRESHOLD 11500

// From timer.cpp ISR
volatile unsigned char ms = 0;

// Set by INT0 button interrupt
volatile bool switchInterruptFlag = false;

// Used for software timing
unsigned long systemMs = 0;
unsigned long lastSerialPrint = 0;
unsigned long lastChirpUpdate = 0;

// Alarm latch
bool alarmTriggered = false;

// Accelerometer values
int16_t accelX = 0;
int16_t accelY = 0;
int16_t accelZ = 0;

// -------------------- Display State Machine --------------------

enum DisplayState {
  DISPLAY_SMILEY,
  DISPLAY_FROWNY
};

DisplayState displayState = DISPLAY_SMILEY;

// -------------------- Debounce State Machine --------------------

enum DebounceState {
  DB_IDLE,
  DB_DEBOUNCE_PRESS,
  DB_WAIT_RELEASE,
  DB_DEBOUNCE_RELEASE
};

DebounceState debounceState = DB_IDLE;
unsigned long debounceStartTime = 0;
bool buttonEvent = false;

// -------------------- Function Prototypes --------------------

// These functions need to exist in spi.cpp
void initSPI();
void displaySmiley();
void displayFrowny();

unsigned char readMPURegister(unsigned char reg);
int16_t combineBytes(unsigned char highByte, unsigned char lowByte);
void readAccelerometer();
void updateDisplayState();
void updateDebounceState();
void updateAlarm();
void wakeUpMPU6050();

// -------------------- Interrupts --------------------

ISR(INT0_vect) {
  switchInterruptFlag = true;
}

// -------------------- Setup --------------------

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

// -------------------- Loop --------------------

void loop() {
  if (ms) {
    ms = 0;
    systemMs++;
  }

  readAccelerometer();

  updateDebounceState();

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

// -------------------- MPU6050 Functions --------------------

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

// -------------------- Display State Machine --------------------

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

// -------------------- Debounce State Machine --------------------

void updateDebounceState() {
  switch (debounceState) {
    case DB_IDLE:
      if (switchInterruptFlag) {
        switchInterruptFlag = false;
        debounceStartTime = systemMs;
        debounceState = DB_DEBOUNCE_PRESS;
      }
      break;

    case DB_DEBOUNCE_PRESS:
      if (systemMs - debounceStartTime >= 25) {
        buttonEvent = true;
        debounceState = DB_WAIT_RELEASE;
      }
      break;

    case DB_WAIT_RELEASE:
      if (switchInterruptFlag) {
        switchInterruptFlag = false;
        debounceStartTime = systemMs;
        debounceState = DB_DEBOUNCE_RELEASE;
      }
      break;

    case DB_DEBOUNCE_RELEASE:
      if (systemMs - debounceStartTime >= 25) {
        debounceState = DB_IDLE;
      }
      break;
  }
}

// -------------------- Alarm / Chirp --------------------

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
