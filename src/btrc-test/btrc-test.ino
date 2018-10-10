#include <SoftwareSerial.h>

#define BT_SPEED     38400  ///< Bluetooth speed rate (baud).
#define BT_TIMEOUT      10  ///< Bluetooth timeout (ms).
#define BT_MAGIC      "JB"  ///< Bluetooth magic signature.

#define PIN_J1 3
#define PIN_J2 5

#define PIN_SW1 2
#define PIN_SW2 4

SoftwareSerial bt(10, 11);

byte leds[] = {12, 9, 8, 7};
const size_t numLeds = sizeof(leds) / sizeof(byte);

byte sign[2];
int data[6 + numLeds];

int pwm(int a)
{
  a = abs(a - 512);
  if (a < 16) a = 0;
  return a / 2;
}

void setup() {
  pinMode(PIN_J1, OUTPUT);
  pinMode(PIN_J2, OUTPUT);

  pinMode(PIN_SW1, OUTPUT);
  pinMode(PIN_SW2, OUTPUT);

  for (auto i = 0; i < numLeds; i++)
    pinMode(leds[i], OUTPUT);

  Serial.begin(BT_SPEED);
  bt.begin(BT_SPEED);
}

void loop() {
  if (bt.available()) {
    bt.readBytes(sign, 2);
    if (sign[0] != 'J' || sign[1] != 'B')
      return;
    bt.readBytes((byte*)data, sizeof(data));

    // Debug output to port monitor
    for (auto i = 0; i < 10; i++) {
      Serial.print(data[i]);
      Serial.print('|');
    }
    Serial.println();

    analogWrite(PIN_J1, pwm(data[0]));
    analogWrite(PIN_J2, pwm(data[1]));

    // Joystick switches
    digitalWrite(PIN_SW1, data[2]);
    digitalWrite(PIN_SW2, data[5]);

    // Buttons
    for (auto i = 0; i < numLeds; i++)
      digitalWrite(leds[i], data[6 + i]);
  }
}

/* vim: set ft=arduino et sw=2 ts=2: */
