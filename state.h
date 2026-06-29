#ifndef STATE_H
#define STATE_H
#include <Arduino.h>

extern bool inMenu;
extern bool editMode;
extern bool btnHolding;
extern bool sleeping;
extern bool webControl;
extern bool manualPWMMode;
/*----- sleep -----*/
extern bool motionDetected;
extern unsigned long lastMotion;

extern unsigned long btnPressStart;
extern unsigned long sleepTimer;
extern unsigned long lastActivity;

extern int maxTemp;
extern int currentTip;

void setTipProfile(int type);

extern uint8_t menuIndex;
extern int lastEncoder;

extern float kp;
extern float ki;
extern float kd;

void setTipProfile(int type);

#endif