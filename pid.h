#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "config.h"
#include "boost.h"

/* ===== PID PARAM ===== */
extern float kp;
extern float ki;
extern float kd;

/* ===== PID VAR ===== */
extern float pidError;
extern float pidIntegral;
extern float pidDerivative;
extern float lastError;

/* ===== TEMP VAR ===== */
extern int targetTemp;
extern int currentTemp;
extern int pwmOut;
extern int tempOffset;

/* ===== SAFETY ===== */
extern bool tipError;
extern bool overHeat;

/* ===== STATUS ===== */
extern uint8_t heaterState;

#define STATE_IDLE  0
#define STATE_HEAT  1
#define STATE_HOLD  2
#define STATE_SLEEP 3
#define STATE_BOOST 4
#define STATE_OVRHT 5
#define STATE_TIP   6

void initPWM();
int readTemp();
void updatePID(bool sleeping);

#endif