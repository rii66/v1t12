#include "state.h"
#include "config.h"
#include "pid.h"
#include "encoder.h"

bool inMenu = false;
bool editMode = false;
bool btnHolding = false;
bool sleeping = false;
bool webControl = false;
bool manualPWMMode = false;
bool motionDetected = false;
unsigned long lastMotion = 0;

unsigned long btnPressStart = 0;
unsigned long sleepTimer = 30000;
unsigned long lastActivity = 0;

int maxTemp = TEMP_MAX_T12;
int currentTip = TIP_T12;

uint8_t menuIndex = 0;
int lastEncoder = 0;

void setTipProfile(int type) {

  currentTip = type;

  if (type == TIP_T12) {
    maxTemp = TEMP_MAX_T12;
    kp = PID_KP_T12;
    ki = PID_KI_T12;
    kd = PID_KD_T12;
  }

  else if (type == TIP_C210) {
    maxTemp = TEMP_MAX_C210;
    kp = PID_KP_C210;
    ki = PID_KI_C210;
    kd = PID_KD_C210;
  }

  if (targetTemp > maxTemp) {
    targetTemp = maxTemp;
  }

  encoderPos = targetTemp;
}