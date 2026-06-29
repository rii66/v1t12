#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "config.h"

extern volatile int encoderPos;
extern volatile bool encoderMoved;

extern volatile uint8_t lastState;
extern volatile unsigned long lastEncInterrupt;

void IRAM_ATTR encoderISR();
void initEncoder();
bool buttonPressed();

#endif