#ifndef BOOST_H
#define BOOST_H

#include <Arduino.h>

/* ===== GLOBAL ===== */
extern bool boostMode;
extern unsigned long boostStart;

extern int boostTemp;
extern int sleepTemp;
extern int savedTemp;

/* ===== FUNC ===== */
void startBoost();
void updateBoost();

#endif