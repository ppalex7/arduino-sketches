#define FIRST_LED_PIN 5
#define LAST_LED_PIN 8

void setup() {
  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; ++pin) {
    pinMode(pin, OUTPUT);
  }

}

int prevPin = LAST_LED_PIN;
void loop() {
  unsigned long int ms = millis();
  int pin = FIRST_LED_PIN + (ms / 120) % (LAST_LED_PIN - FIRST_LED_PIN + 1);
  if (prevPin != pin) {
    digitalWrite(prevPin, LOW);
    digitalWrite(pin, HIGH);
    prevPin = pin;
  }
}