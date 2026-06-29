#ifndef EFFECTS_H
#define EFFECTS_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "config.h"
#include "boost.h"
#include "pid.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void bootFlame() {
  for (int i = 0; i < 4; i++) {
    u8g2.clearBuffer();

    u8g2.drawTriangle(18,40,25,15,32,40);
    u8g2.drawDisc(25,28,4);

    u8g2.drawTriangle(53,40,60,10,67,40);
    u8g2.drawDisc(60,24,5);

    u8g2.drawTriangle(88,40,95,15,102,40);
    u8g2.drawDisc(95,28,4);

    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.setCursor(28,58);
    u8g2.print("FIRE UP");

    u8g2.sendBuffer();

    tone(BUZZER_PIN,1500,80);
    delay(180);

    u8g2.clearBuffer();

    u8g2.drawTriangle(20,40,25,18,30,40);
    u8g2.drawTriangle(55,40,60,14,65,40);
    u8g2.drawTriangle(90,40,95,18,100,40);

    u8g2.setCursor(20,58);
    u8g2.print("IGNITION");

    u8g2.sendBuffer();

    delay(180);
  }
}

void updateRGB() {
  static bool blinkState = false;
  static unsigned long lastBlink = 0;

  if (boostMode) {
    if (millis() - lastBlink > 250) {
      blinkState = !blinkState;
      lastBlink = millis();
    }

    digitalWrite(RGB_RED, blinkState ? LOW : HIGH);
    digitalWrite(RGB_BLUE, blinkState ? LOW : HIGH);
    return;
  }

  if (currentTemp < targetTemp - 20) {
    digitalWrite(RGB_RED, HIGH);
    digitalWrite(RGB_BLUE, LOW);
  }
  else {
    digitalWrite(RGB_RED, LOW);
    digitalWrite(RGB_BLUE, HIGH);
  }
}

#endif