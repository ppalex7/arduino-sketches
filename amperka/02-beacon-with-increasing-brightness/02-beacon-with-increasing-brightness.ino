#define LED_PIN 9

void setup() {
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(LED_PIN, 85);
  delay(250);

  analogWrite(LED_PIN, 170);
  delay(250);

  analogWrite(LED_PIN, 255);
  delay(250);
}
