#include <SoftwareSerial.h>

#define BT_SPEED 38400
#define PIN_STATE 2

SoftwareSerial bt(10, 11);
volatile bool bt_state = false;

const byte a[] = {3, 5, 6, 9};
const byte b[] = {4, 7, A0, A1, A2, A3};

const size_t size_a = sizeof(a) / sizeof(byte);
const size_t size_b = sizeof(b) / sizeof(byte);

struct {
  uint8_t  a[size_a];
  uint16_t b;
} data;

void setup() {
  attachInterrupt(digitalPinToInterrupt(PIN_STATE), bt_state_changed, CHANGE);
  bt_state_changed();

  while (!bt_state);

  for (auto i = 0; i < size_a; i++)
    pinMode(a[i], OUTPUT);
  for (auto i = 0; i < size_b; i++)
    pinMode(b[i], OUTPUT);

  Serial.begin(BT_SPEED);
  bt.begin(BT_SPEED);
}

void loop() {
  if (!bt_state || !bt.available() ||
      bt.read() != 'B' || bt.read() != 'T' ||
      bt.readBytes((byte*)&data, sizeof(data)) != sizeof(data)) return;
  for (auto i = 0; i < size_a; i++)
    analogWrite(a[i], data.a[i] >> 1);
  for (auto i = 0; i < size_b; i++)
    digitalWrite(b[i], data.b & (1 << i));
}

void bt_state_changed() {
  bt_state = (bool)digitalRead(PIN_STATE);
}
