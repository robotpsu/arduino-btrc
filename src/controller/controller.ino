#include <SoftwareSerial.h>

#define BT_SPEED 38400
#define BT_DELAY 40
#define PIN_STATE 2

SoftwareSerial bt(10, 11);
volatile bool bt_state = false;

const byte a[] = {A0, A1, A2, A3};
const byte b[] = {8, 9, 4, 5, 6, 7};

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

  for (auto i = 0; i < size_b; i++)
    pinMode(b[i], INPUT_PULLUP);

  Serial.begin(BT_SPEED);
  bt.begin(BT_SPEED);
}

void loop() {
  if (!bt_state) return;

  data.b = 0;
  for (auto i = 0; i < size_a; i++)
    data.a[i] = analogRead(a[i]) >> 2;
  for (auto i = 0; i < size_b; i++)
    data.b |= !digitalRead(b[i]) << i;

  bt.write('B'); bt.write('T');
  bt.write((byte*)&data, sizeof(data));

  delay(BT_DELAY);
}

void bt_state_changed() {
  bt_state = (bool)digitalRead(PIN_STATE);
}
