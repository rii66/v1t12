#include "motion.h"
#include "config.h"
#include "state.h"
#include "pid.h"
#include "encoder.h"

void initMotion() {
  lastMotion = millis();
}

void updateMotion() {

  // encoder bergerak = dianggap motion
  if (encoderPos != lastEncoder) {
    motionDetected = true;
    lastMotion = millis();

    if (sleeping) {
      sleeping = false;
      targetTemp = encoderPos;
    }

    lastEncoder = encoderPos;
  }

  // auto sleep kalau idle
  if (
    sleepTimer > 0 &&
    millis() - lastMotion > sleepTimer
  ) {
    sleeping = true;
  }
}