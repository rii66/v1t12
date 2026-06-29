#include <WebSocketsServer.h>
#include <WiFi.h>
#include "config.h"
#include "boost.h"
#include "pid.h"
#include "state.h"
#include "encoder.h"
#include "storage.h"

extern WebSocketsServer webSocket;

void onWebSocketEvent(
  uint8_t num,
  WStype_t type,
  uint8_t *payload,
  size_t length
) {
  if (type != WStype_TEXT) return;

  String msg = String((char*)payload);

  Serial.println("RX: " + msg);

  /* SET TEMP */
  if (msg.startsWith("SET:")) {
    targetTemp = constrain(
      msg.substring(4).toInt(),
      TEMP_MIN,
      maxTemp
    );

    encoderPos = targetTemp;
    webControl = true;
    manualPWMMode = false;

    saveSettings();
  }

  /* MANUAL PWM */
  else if (msg.startsWith("PWM:")) {
    manualPWMMode = true;

    int pwmPercent = constrain(
      msg.substring(4).toInt(),
      0,
      100
    );

    pwmOut = map(
      pwmPercent,
      0, 100,
      0, 255
    );

    analogWrite(PWM_PIN, pwmOut);
  }

  /* BOOST */
  else if (msg == "BOOST") {
    startBoost();
  }

  /*/* MAX TEMP LIMIT */
  else if (msg.startsWith("MAXTEMP:")) {
    maxTemp = constrain(
     msg.substring(8).toInt(),
     TEMP_MIN,
     TEMP_MAX
   );

    if (targetTemp > maxTemp) {
     targetTemp = maxTemp;
     encoderPos = targetTemp;
   }

    webControl = true;

    saveSettings();
  }

  /* SLEEP TIME */
  else if (msg.startsWith("SLEEPTIME:")) {
    sleepTimer = msg.substring(10).toInt();
    saveSettings();
  }

  /* PID SAVE */
  else if (msg.startsWith("PID:")) {
    String data = msg.substring(4);

    int p1 = data.indexOf(',');
    int p2 = data.lastIndexOf(',');

    kp = data.substring(0, p1).toFloat();
    ki = data.substring(p1 + 1, p2).toFloat();
    kd = data.substring(p2 + 1).toFloat();

    saveSettings();
  }

  /* TIP PROFILE */
  else if (msg.startsWith("TIP:")) {
    String tip = msg.substring(4);

    if (tip == "T12")
      setTipProfile(TIP_T12);

    else if (tip == "C210")
      setTipProfile(TIP_C210);

    saveSettings();
  }

  lastActivity = millis();
}

void initWebSocket() {
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void sendLiveData() {
  String packet = "{";

  packet += "\"temp\":" + String(currentTemp) + ",";
  packet += "\"set\":" + String(targetTemp) + ",";
  packet += "\"pwm\":" + String(map(pwmOut, 0, 255, 0, 100)) + ",";
  packet += "\"wifi\":" +
            String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";

  packet += "\"tip\":\"";
  packet += (currentTip == TIP_T12 ? "T12" : "C210");
  packet += "\",";

  packet += "\"state\":\"";

  if (boostMode)
    packet += "BOOST";
  else if (sleeping)
    packet += "SLEEP";
  else if (manualPWMMode)
    packet += "MANUAL";
  else
    packet += "PID";

  packet += "\"}";

  webSocket.broadcastTXT(packet);
}