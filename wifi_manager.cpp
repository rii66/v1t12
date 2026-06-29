#include <WiFi.h>
#include "config.h"

void setupWiFi() {
  WiFi.mode(WIFI_AP_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  WiFi.softAP(AP_SSID, AP_PASS);

  Serial.println("Connecting WiFi...");

  unsigned long start = millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - start < 10000
  ) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("STA IP: ");
    Serial.println(WiFi.localIP());
  }

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}