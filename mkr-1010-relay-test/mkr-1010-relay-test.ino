static const pin_size_t pins[] = {A0, A1, A2, A5, A6, PIN_LED};
// static const pin_size_t pins[] = {0, 1, 2, 3, 4, 7};
#define pins_cnt (sizeof(pins) / sizeof(pins[0]))
#define INITAL_BITS 0b111111 << 6

static short bits = INITAL_BITS;

void setup() {
  for (byte i = 0; i < pins_cnt; i++) {
    pin_size_t pin = pins[i];
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  for (byte i = 0; i < pins_cnt; i++) {
    digitalWrite(pins[i], bitRead(bits, i));
  }
  delay(167);
  bits = bits >> 1;
  if (bits == (INITAL_BITS >> 7) || bits == (INITAL_BITS >> 1)) {
    delay(2000);
  }
  if (bits == 0) {
    bits = INITAL_BITS;
  }
}
