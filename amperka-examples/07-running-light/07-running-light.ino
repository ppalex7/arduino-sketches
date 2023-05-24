#define FIRST_LED_PIN 2
#define LAST_LED_PIN 11

void setup() {
  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; ++pin) {
    pinMode(pin, OUTPUT);
  }

}

void loop() {
  for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; ++pin) {
    digitalWrite(pin, HIGH);
    delay(14);
    digitalWrite(pin, LOW);
    delay(10);
  }
}