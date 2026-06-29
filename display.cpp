#include <Wire.h>
#include <U8g2lib.h>
#include "config.h"
#include "state.h"
#include "pid.h"
#include "boost.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

/* ===== MENU PAGE ===== */
enum {
  PAGE_SET,
  PAGE_BOOST,
  PAGE_SLEEP,
  PAGE_CAL,
  PAGE_PID,
  PAGE_TOTAL
};

int animX = 0;
int targetX = 0;

/* ===== ICONS ===== */

void drawSetPage(int x) {
  u8g2.drawFrame(x + 20, 8, 88, 48);

  u8g2.drawCircle(x + 64, 28, 10);
  u8g2.drawLine(x + 64, 18, x + 64, 38);

  u8g2.setCursor(x + 42, 52);
  u8g2.print(targetTemp);
  u8g2.print("C");

  u8g2.drawStr(x + 45, 62, "SET");
}

void drawBoostPage(int x) {
  u8g2.drawFrame(x + 20, 8, 88, 48);

  u8g2.drawTriangle(
    x + 55, 18,
    x + 70, 18,
    x + 58, 36
  );

  u8g2.drawTriangle(
    x + 60, 30,
    x + 74, 30,
    x + 62, 48
  );

  u8g2.setCursor(x + 42, 52);
  u8g2.print(boostTemp);
  u8g2.print("C");

  u8g2.drawStr(x + 38, 62, "BOOST");
}

void drawSleepPage(int x) {
  u8g2.drawFrame(x + 20, 8, 88, 48);

  u8g2.drawCircle(x + 60, 28, 10);
  u8g2.drawDisc(x + 64, 24, 8);

  u8g2.setCursor(x + 42, 52);
  u8g2.print(sleepTemp);
  u8g2.print("C");

  u8g2.drawStr(x + 40, 62, "SLEEP");
}

void drawCalPage(int x) {
  u8g2.drawFrame(x + 20, 8, 88, 48);

  u8g2.drawCircle(x + 64, 28, 10);
  u8g2.drawLine(x + 54, 28, x + 74, 28);
  u8g2.drawLine(x + 64, 18, x + 64, 38);

  u8g2.setCursor(x + 42, 52);
  u8g2.print(tempOffset);

  u8g2.drawStr(x + 48, 62, "CAL");
}

void drawPIDPage(int x) {
  u8g2.drawFrame(x + 20, 8, 88, 48);

  u8g2.drawLine(x + 40, 20, x + 85, 20);
  u8g2.drawLine(x + 40, 30, x + 85, 30);
  u8g2.drawLine(x + 40, 40, x + 85, 40);

  u8g2.drawDisc(x + 55, 20, 2);
  u8g2.drawDisc(x + 70, 30, 2);
  u8g2.drawDisc(x + 60, 40, 2);

  u8g2.drawStr(x + 48, 62, "PID");
}

/* ===== DOTS ===== */

void drawDots() {
  for (int i = 0; i < PAGE_TOTAL; i++) {
    if (i == menuIndex)
      u8g2.drawDisc(48 + (i * 8), 63, 2);
    else
      u8g2.drawCircle(48 + (i * 8), 63, 2);
  }
}

/* ===== DISPLAY ===== */

void initDisplay() {
  Wire.begin(OLED_SDA, OLED_SCL);
  u8g2.begin();
}

void drawUI() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  if (inMenu) {
    targetX = menuIndex * 128;

    /* smooth slide */
    animX += (targetX - animX) / 4;

    /* status bar */
    u8g2.drawStr(0, 8, "MENU");

    if (editMode)
      u8g2.drawStr(92, 8, "EDIT");

    /* pages */
    drawSetPage(0 - animX);
    drawBoostPage(128 - animX);
    drawSleepPage(256 - animX);
    drawCalPage(384 - animX);
    drawPIDPage(512 - animX);

    drawDots();
  }

  else {
    u8g2.drawStr(0, 12, "SOLDER STATION");

    if (boostMode)
      u8g2.drawStr(90, 12, "BST");

    if (sleeping)
      u8g2.drawStr(90, 22, "SLP");

    if (tipError)
      u8g2.drawStr(90, 32, "TIP!");

    if (overHeat)
      u8g2.drawStr(90, 42, "HOT!");

    u8g2.setCursor(0, 28);
    u8g2.print("SET:");
    u8g2.print(targetTemp);

    u8g2.setCursor(0, 44);
    u8g2.print("TMP:");
    u8g2.print(currentTemp);

    u8g2.setCursor(0, 60);
    u8g2.print("PWM:");
    u8g2.print(map(pwmOut, 0, 255, 0, 100));
  }

  u8g2.sendBuffer();
}