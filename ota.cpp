#include <ArduinoOTA.h>
#include "config.h"

void setupOTA() {
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.begin();
}