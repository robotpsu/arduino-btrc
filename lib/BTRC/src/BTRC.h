/**
 * \file
 * \brief   Blutooth Remote Controller.
 * \author  Vasiliy Polyakov <vp@psu.ru>
 * \author  RobotPSU https://robot.psu.ru/
 * \date    2018
 */

#ifndef BTRC_h
#define BTRC_h

#include <Arduino.h>
#include <Joystick.h>

#ifndef BTRC_JOYSTICKS
#define BTRC_JOYSTICKS 2
#endif

#ifndef BTRC_BUTTONS
#define BTRC_BUTTONS 4
#endif

const long BTRC_SPEED   = 38400;  ///< Bluetooth speed rate (baud).
const int  BTRC_TIMEOUT =    10;  ///< Bluetooth timeout (ms).

const size_t BTRC_MAGIC_LEN = 4;
const char BTRC_MAGIC[BTRC_MAGIC_LEN] = "BTRC";  ///< Bluetooth magic signature.

struct BTRCData {
  JoystickData j[BTRC_JOYSTICKS];
  byte b[BTRC_BUTTONS];
};
const size_t BTRC_DATA_LEN = sizeof(BTRCData);

#endif

/* vim: set ft=arduino et sw=2 ts=2: */
