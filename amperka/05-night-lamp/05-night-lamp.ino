#define LED_PIN 13
#define LED2_PIN 12
#define LDR_PIN A1
#define POT_PIN A0

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void loop() {
  int lightness = analogRead(LDR_PIN);
  int threshold = analogRead(POT_PIN);

  digitalWrite(LED_PIN, lightness < threshold ? HIGH : LOW);
  digitalWrite(LED2_PIN, (lightness < threshold / 2) ? HIGH : LOW);
}
