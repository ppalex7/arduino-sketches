#define LED_PIN 11
#define LED2_PIN 10
#define LDR_PIN A1
#define POT_PIN A0

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void loop() {
  int lightness = analogRead(LDR_PIN);
  int threshold = analogRead(POT_PIN);

  long brightness = map(lightness, 1023, 0, 0, 255);

  analogWrite(LED_PIN, lightness < threshold ? brightness : 0);
  analogWrite(LED2_PIN, (lightness < threshold / 2) ? brightness : 0);
}
