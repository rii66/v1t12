#include "encoder.h"

volatile int encoderPos = DEFAULT_TEMP;
volatile bool encoderMoved = false;

volatile uint8_t lastState = 0;
volatile unsigned long lastEncInterrupt = 0;

void IRAM_ATTR encoderISR() {
  unsigned long now = micros();

  if (now - lastEncInterrupt < 3000)
    return;

  lastEncInterrupt = now;

  uint8_t a = digitalRead(ENC_A);
  uint8_t b = digitalRead(ENC_B);

  uint8_t encoded = (a << 1) | b;
  uint8_t sum = (lastState << 2) | encoded;

  if (
    sum == 0b1101 ||
    sum == 0b0100 ||
    sum == 0b0010 ||
    sum == 0b1011
  ) {
    encoderPos++;
  }

  if (
    sum == 0b1110 ||
    sum == 0b0111 ||
    sum == 0b0001 ||
    sum == 0b1000
  ) {
    encoderPos--;
  }

  encoderMoved = true;
  lastState = encoded;
}

void initEncoder() {
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  lastState =
    (digitalRead(ENC_A) << 1) |
    digitalRead(ENC_B);

  attachInterrupt(
    digitalPinToInterrupt(ENC_A),
    encoderISR,
    CHANGE
  );

  attachInterrupt(
    digitalPinToInterrupt(ENC_B),
    encoderISR,
    CHANGE
  );
}

bool buttonPressed() {
  return !digitalRead(ENC_SW);
}