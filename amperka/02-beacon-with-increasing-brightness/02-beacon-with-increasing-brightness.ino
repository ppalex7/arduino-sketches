#define LED_PIN 9

unsigned int voltage = 1;
const unsigned int maxVoltage = 5;

void setup() {
}

void loop() {
  // put your main code here, to run repeatedly:
  int output = 255 * voltage / maxVoltage;
  analogWrite(LED_PIN, output);
  delay(166);

  if (++voltage > maxVoltage) {
    voltage = 0;
  }
}
