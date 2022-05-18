#define CONTROL_PIN A0
#define LED_PIN 9

void setup() {
  pinMode(CONTROL_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int rotation, brightness;

  rotation = analogRead(CONTROL_PIN);

  brightness = rotation / 4;

  analogWrite(LED_PIN, brightness);
}
