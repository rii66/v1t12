#include "boost.h"
#include "config.h"
#include "pid.h"
#include "encoder.h"

bool boostMode = false;
unsigned long boostStart = 0;

int boostTemp = BOOST_TEMP;
int sleepTemp = SLEEP_TEMP;

int savedTemp = DEFAULT_TEMP;

void startBoost() {
  if (!boostMode) {
    savedTemp = targetTemp;
    boostMode = true;
    boostStart = millis();
  }
}

void updateBoost() {
  if (
    boostMode &&
    (millis() - boostStart > BOOST_TIME)
  ) {
    boostMode = false;

    targetTemp = savedTemp;
    encoderPos = savedTemp;
  }
}