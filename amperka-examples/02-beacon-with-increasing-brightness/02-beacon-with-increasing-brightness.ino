#define LED_PIN 9
#define LED2_PIN 3

unsigned int voltage = 1;
const unsigned int maxVoltage = 5;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int output = 255 * voltage / maxVoltage;
  int output2 = 255 * (maxVoltage - voltage) / maxVoltage;
  Serial.print("v1: ");
  Serial.print(output);
  Serial.print(", v2: ");
  Serial.print(output2);
  Serial.println();
  analogWrite(LED_PIN, output);
  analogWrite(LED2_PIN, output2);
  delay(166);

  if (++voltage > maxVoltage) {
    voltage = 0;
  }
}
