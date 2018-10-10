/**
 * \file
 * \brief   Bluetooth remote controller for Arduino.
 * \author  Vasiliy Polyakov <vp@psu.ru>
 * \author  RobotPSU https://robot.psu.ru/
 * \date    2018
 */

#include <Joystick.h>
#include <SoftwareSerial.h>

#define BT_SPEED     38400  ///< Bluetooth speed rate (baud).
#define BT_TIMEOUT      10  ///< Bluetooth timeout (ms).
#define BT_MAGIC      "JB"  ///< Bluetooth magic signature.

#define PIN_X1 A0  ///< Pin X of the 1st joystick (analog).
#define PIN_Y1 A1  ///< Pin Y of the 1st joystick (analog).
#define PIN_SW1 2  ///< Pin SW of the 1st joystick (digital).

#define PIN_X2 A2  ///< Pin X of the 2nd joystick (analog).
#define PIN_Y2 A3  ///< Pin Y of the 2nd joystick (analog).
#define PIN_SW2 3  ///< Pin SW of the 2nd joystick (digital).

#define BUTTONS {4, 5, 6, 7}  ///< List of button pins.

SoftwareSerial bt(10, 11);

Joystick j1, j2;

byte buttons[] = BUTTONS;
const size_t numButtons = sizeof(buttons) / sizeof(byte);

int data[6 + numButtons];

void setup() {
  j1.attach(PIN_X1, PIN_Y1, PIN_SW1);
  j2.attach(PIN_X2, PIN_Y2, PIN_SW2);

  for (auto i = 0; i < numButtons; i++)
    pinMode(buttons[i], INPUT_PULLUP);

  Serial.begin(BT_SPEED);
  bt.begin(BT_SPEED);
}

void loop() {
  size_t n = 0;

  data[n++] = j1.x();
  data[n++] = j1.y();
  data[n++] = j1.s();

  data[n++] = j2.x();
  data[n++] = j2.y();
  data[n++] = j2.s();

  for (auto i = 0; i < numButtons; i++)
    data[n++] = !digitalRead(buttons[i]);

  for (auto i = 0; i < n; i++) {
    Serial.print(data[i]);
    Serial.print('|');
  }
  Serial.println();

  bt.write(BT_MAGIC);
  bt.write((byte*)data, sizeof(data));

  delay(BT_TIMEOUT);
}

/* vim: set ft=arduino et sw=2 ts=2: */
