#define FIRST_LED_PIN 2
#define LAST_LED_PIN 11

void setup() {
  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; ++pin) {
    pinMode(pin, OUTPUT);
  }

}

int prevPin = LAST_LED_PIN;
void loop() {
  unsigned long int ms = millis();
  int pin = FIRST_LED_PIN + (ms / 120) % 10;
  if (prevPin != pin) {
    digitalWrite(prevPin, LOW);
    digitalWrite(pin, HIGH);
    prevPin = pin;
  }
}