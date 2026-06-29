#include "pid.h"

/* ===== PID PARAM ===== */
float kp = PID_KP_T12;
float ki = PID_KI_T12;
float kd = PID_KD_T12;

/* ===== PID VAR ===== */
float pidError = 0;
float pidIntegral = 0;
float pidDerivative = 0;
float lastError = 0;

/* ===== TEMP VAR ===== */
int targetTemp = DEFAULT_TEMP;
int currentTemp = 0;
int pwmOut = 0;
int tempOffset = 0;

/* ===== SAFETY ===== */
bool tipError = false;
bool overHeat = false;

/* ===== STATUS ===== */
uint8_t heaterState = STATE_IDLE;

/* ===== PWM INIT ===== */
void initPWM() {
  pinMode(PWM_PIN, OUTPUT);
  analogWrite(PWM_PIN, 0);

  analogSetPinAttenuation(TEMP_PIN, ADC_11db);
}

/* ===== TEMP READ ===== */
int readTemp() {
  long total = 0;

  for (int i = 0; i < 16; i++) {
    total += analogRead(TEMP_PIN);
    delayMicroseconds(150);
  }

  int raw = total / 16;
  Serial.println(raw);

  if (raw < 5) {
    tipError = true;
    analogWrite(PWM_PIN, 0);
    return 0;
  }

  tipError = false;

  int temp = map(raw, 0, 4095, 20, 500);
  temp += tempOffset;

  return temp;
}

/* ===== PID UPDATE (NO DEFAULT ARG) ===== */
void updatePID(bool sleeping) {
  currentTemp = readTemp();

  /* OVERHEAT */
  if (currentTemp >= 450) {
    overHeat = true;
    pwmOut = 0;
    pidIntegral = 0;
    analogWrite(PWM_PIN, 0);
    heaterState = STATE_OVRHT;
    return;
  }

  overHeat = false;

  /* TIP ERROR */
  if (tipError) {
    pwmOut = 0;
    pidIntegral = 0;
    analogWrite(PWM_PIN, 0);
    heaterState = STATE_TIP;
    return;
  }

  int activeTarget = targetTemp;

  if (sleeping) {
    activeTarget = sleepTemp;
  }

  if (boostMode) {
    activeTarget = boostTemp;
  }

  /* PID */
  pidError = activeTarget - currentTemp;

  pidIntegral += pidError;
  pidIntegral = constrain(pidIntegral, -500, 500);

  pidDerivative = pidError - lastError;

  float output = (kp * pidError) +
                 (ki * pidIntegral) +
                 (kd * pidDerivative);

  pwmOut = constrain((int)output, 0, 255);
  if (pwmOut > 0 && pwmOut < 35)
  pwmOut = 35;

  analogWrite(PWM_PIN, pwmOut);

  lastError = pidError;

  /* STATUS */
  if (boostMode) heaterState = STATE_BOOST;
  else if (sleeping) heaterState = STATE_SLEEP;
  else if (currentTemp < activeTarget - 15) heaterState = STATE_HEAT;
  else heaterState = STATE_HOLD;
}