/**
 * \file
 * \brief   Bluetooth remote controller for Arduino.
 * \author  Vasiliy Polyakov <vp@psu.ru>
 * \author  RobotPSU https://robot.psu.ru/
 * \date    2018
 */

#define DEBUG 1

#include <BTRC.h>

#ifdef DEBUG
#include <SoftwareSerial.h>
SoftwareSerial BTRC(10, 11);
#else
#define BTRC Serial
#endif

#define PIN_J1_X A0  ///< Pin X of the 1st joystick (analog).
#define PIN_J1_Y A1  ///< Pin Y of the 1st joystick (analog).
#define PIN_J1_SW 2  ///< Pin SW of the 1st joystick (digital).

#define PIN_J2_X A2  ///< Pin X of the 2nd joystick (analog).
#define PIN_J2_Y A3  ///< Pin Y of the 2nd joystick (analog).
#define PIN_J2_SW 3  ///< Pin SW of the 2nd joystick (digital).

Joystick j[BTRC_JOYSTICKS];
const byte b[BTRC_BUTTONS] = {4, 5, 6, 7};
BTRCData data;

void setup() {
  j[0].attach(PIN_J1_X, PIN_J1_Y, PIN_J1_SW);
  j[1].attach(PIN_J2_X, PIN_J2_Y, PIN_J2_SW);

  for (auto i = 0; i < BTRC_BUTTONS; i++)
    pinMode(b[i], INPUT_PULLUP);

  BTRC.begin(BTRC_SPEED);
  #ifdef DEBUG
  Serial.begin(BTRC_SPEED);
  #endif
}

void loop() {
  // Get joysticks position and switch state
  for (auto i = 0; i < BTRC_JOYSTICKS; i++) {
    data.j[i].x = j[i].x();
    data.j[i].y = j[i].y();
    data.j[i].s = j[i].s();
  }

  // Get buttons state
  for (auto i = 0; i < BTRC_BUTTONS; i++)
    data.b[i] = !digitalRead(b[i]);

  #ifdef DEBUG
  for (auto i = 0; i < BTRC_JOYSTICKS; i++) {
    Serial.print(data.j[i].x);
    Serial.print('x');
    Serial.print(data.j[i].y);
    Serial.print(';');
    Serial.print(data.j[i].s);
    Serial.print('|');
  }
  for (auto i = 0; i < BTRC_BUTTONS; i++) {
    Serial.print(data.b[i]);
    Serial.print('|');
  }
  Serial.println();
  #endif

  // Send data
  BTRC.write(BTRC_MAGIC, BTRC_MAGIC_LEN);
  BTRC.write((byte*)&data, BTRC_DATA_LEN);

  delay(BTRC_TIMEOUT);
}

/* vim: set ft=arduino et sw=2 ts=2: */
