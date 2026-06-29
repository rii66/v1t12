#include "menu.h"
#include "state.h"
#include "encoder.h"
#include "boost.h"
#include "pid.h"
#include "buzzer.h"
#include "storage.h"

void handleButton() {

  /* tombol ditekan */
  if (buttonPressed()) {

    if (!btnHolding) {
      btnHolding = true;
      btnPressStart = millis();
    }

    /* long press = masuk / keluar menu */
    if (millis() - btnPressStart >= 2100) {
      inMenu = !inMenu;
      editMode = false;
      btnHolding = false;

      beep(120);
      delay(300);
    }
  }

  /* tombol dilepas */
  else {
    if (btnHolding) {

      unsigned long pressTime =
        millis() - btnPressStart;

      /* short press normal */
      if (pressTime < 700 && !inMenu) {
        startBoost();

        sleeping = false;
        lastActivity = millis();

        beep(40);
      }

      /* short press di menu */
      if (pressTime < 1000 && inMenu) {

        /* masuk / keluar edit */
        if (!editMode) {
          editMode = true;
          beep(60);
        }
        else {
          editMode = false;
          saveSettings();
          beep(80);
        }
      }

      btnHolding = false;
    }
  }
}

void handleMenu() {

  /* encoder gerak */
  if (encoderPos != lastEncoder) {
    webControl = false;
    sleeping = false;
    lastActivity = millis();
    lastEncoder = encoderPos;
  }

  /* auto sleep */
  unsigned long idleBase =
    (lastMotion > lastActivity)
    ? lastMotion
    : lastActivity;

  if (
    sleepTimer > 0 &&
    millis() - lastActivity > sleepTimer
  ) {
    sleeping = true;
  }

  /* menu mode */
  if (inMenu) {

    /* pilih halaman */
    if (!editMode) {
      menuIndex =
        ((encoderPos % 5) + 5) % 5;
    }

    /* edit value */
    else {
      switch (menuIndex) {

        case 0:
          targetTemp = constrain(
            encoderPos,
            TEMP_MIN,
            maxTemp
          );
          break;

        case 1:
          boostTemp = constrain(
            encoderPos,
            TEMP_MIN,
            maxTemp
          );
          break;

        case 2:
          sleepTemp = constrain(
            encoderPos,
            TEMP_MIN,
            maxTemp
          );
          break;

        case 3:
          tempOffset = constrain(
            encoderPos,
            -50,
            50
          );
          break;

        case 4:
          kp = encoderPos / 10.0;
          break;
      }
    }
  }

  /* normal mode */
  else {

  /* kalau encoder diputar, ambil alih dari web */
  if (encoderPos != lastEncoder) {
    webControl = false;
  }

  /* mode encoder */
  if (!boostMode && !webControl) {
    targetTemp = constrain(
      encoderPos,
      TEMP_MIN,
      maxTemp
    );
  }

  /* mode web */
  if (webControl) {
    encoderPos = targetTemp;
  }
 }
}