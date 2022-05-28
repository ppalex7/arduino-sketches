#define LED_PIN 13
#define LDR_PIN A1
#define POT_PIN A0

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int lightness = analogRead(LDR_PIN);
  int threshold = analogRead(POT_PIN);

  digitalWrite(LED_PIN, lightness < threshold ? HIGH : LOW);
}
