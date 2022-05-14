#define BEACON_PIN 9

void setup() {
  // put your setup code here, to run once:
  pinMode(BEACON_PIN, OUTPUT);

  digitalWrite(BEACON_PIN, HIGH);
  delay(3000-375);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(BEACON_PIN, HIGH);
  delay(375);

  digitalWrite(BEACON_PIN, LOW);
  delay(1125);
}
