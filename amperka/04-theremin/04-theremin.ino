#define LDR_PIN A0
#define BUZZER_PIN 3

#define MORSE_DOT_DURATION 110
#define MORSE_DASH_DURATION 3 * MORSE_DOT_DURATION
#define MORSE_FREQ 3000

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  s();
  o();
  s();
  delay(1000);
}

void dot() {
  tone(BUZZER_PIN, MORSE_FREQ, MORSE_DOT_DURATION);
  delay(2 * MORSE_DOT_DURATION);
}

void dash() {
  tone(BUZZER_PIN, MORSE_FREQ, MORSE_DASH_DURATION);
  delay(MORSE_DASH_DURATION + MORSE_DOT_DURATION);
}

void s() {
  for (int i = 0; i < 3; i++) {
    dot();
  }
}
void o() {
  for (int i = 0; i < 3; i++) {
    dash();
  }
}

void loop() {
  int val, frequency;

  val = analogRead(LDR_PIN);

  frequency = map(val, 0, 1023, 3500, 4500);

  tone(BUZZER_PIN, frequency, 20);
}
