/**
 * \file
 * \brief   Bluetooth remote controller test receiver.
 * \author  Vasiliy Polyakov <vp@psu.ru>
 * \author  RobotPSU https://robot.psu.ru/
 * \date    2018
 */

#include <BTRC.h>

const JoystickPins j[BTRC_JOYSTICKS] = {{5, 6, 4}, {9, 10, 8}};
const byte leds[BTRC_BUTTONS] = {2, 3, 7, 11};

char magic[BTRC_MAGIC_LEN];
BTRCData data;

int pwm(int a) {
  a = abs(a - 512);
  if (a < 16) a = 0;
  return a / 2;
}

void setup() {
  for (auto i = 0; i < BTRC_JOYSTICKS; i++) {
    pinMode(j[i].x, OUTPUT);
    pinMode(j[i].y, OUTPUT);
    pinMode(j[i].s, OUTPUT);
  }
  for (auto i = 0; i < BTRC_BUTTONS; i++)
    pinMode(leds[i], OUTPUT);

  Serial.begin(BTRC_SPEED);
}

void loop() {
  if (Serial.available()) {
    // Check magic signature
    Serial.readBytes(magic, BTRC_MAGIC_LEN);
    if (strncmp(magic, BTRC_MAGIC, BTRC_MAGIC_LEN)) return;

    // Get data
    Serial.readBytes((byte*)&data, BTRC_DATA_LEN);

    // Handle joysticks data
    for (auto i = 0; i < BTRC_JOYSTICKS; i++) {
      analogWrite(j[i].x, pwm(data.j[i].x));
      analogWrite(j[i].y, pwm(data.j[i].y));
      digitalWrite(j[i].s, data.j[i].s);
    }

    // Handle buttons data
    for (auto i = 0; i < BTRC_BUTTONS; i++)
      digitalWrite(leds[i], data.b[i]);
  }
}

/* vim: set ft=arduino et sw=2 ts=2: */
