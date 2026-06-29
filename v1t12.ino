#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Update.h>
#include <Preferences.h>

#include "config.h"
#include "boost.h"
#include "pid.h"
#include "encoder.h"
#include "rgb.h"
#include "web.h"
#include "webserver.h"
#include "motion.h"
#include "state.h"
#include "display.h"
#include "buzzer.h"
#include "wifi_manager.h"
#include "ota.h"
#include "websocket_handler.h"
#include "menu.h"
#include "storage.h"
#include "auth.h"

WebServer server(80);
WebSocketsServer webSocket(81);

void initWebServer();

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);

void setup() {
  Serial.begin(115200);
  initStorage();
  loadSettings();
  encoderPos = targetTemp;
  lastEncoder = encoderPos;
  initEncoder();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);

  initPWM();
  initDisplay();

  bootFlame();
  initMotion();

  setupWiFi();
  setupOTA();

  initWebServer();
  initWebSocket();

  lastActivity = millis();

  beep();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  webSocket.loop();

  updateBoost();

  handleButton();
  handleMenu();
  
  updateMotion();

  updatePID(sleeping);
  updateRGB();

  drawUI();

  sendLiveData();

  delay(80);
}
