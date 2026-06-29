#include <Preferences.h>
#include "storage.h"
#include "pid.h"
#include "boost.h"
#include "state.h"

Preferences prefs;

void initStorage() {
  prefs.begin("solder", false);
}

void loadSettings() {
  targetTemp = prefs.getInt("target", 320);
  boostTemp = prefs.getInt("boost", 380);
  sleepTemp = prefs.getInt("sleep", 180);
  tempOffset = prefs.getInt("offset", 0);

  kp = prefs.getFloat("kp", kp);
  ki = prefs.getFloat("ki", ki);
  kd = prefs.getFloat("kd", kd);

  currentTip = prefs.getInt("tip", TIP_T12);
  maxTemp = prefs.getInt("max", TEMP_MAX_T12);
}

void saveSettings() {
  prefs.putInt("target", targetTemp);
  prefs.putInt("boost", boostTemp);
  prefs.putInt("sleep", sleepTemp);
  prefs.putInt("offset", tempOffset);

  prefs.putFloat("kp", kp);
  prefs.putFloat("ki", ki);
  prefs.putFloat("kd", kd);

  prefs.putInt("tip", currentTip);
  prefs.putInt("max", maxTemp);
}