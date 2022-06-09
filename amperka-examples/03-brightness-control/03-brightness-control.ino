#define CONTROL_PIN A0
#define LED_PIN 9
#define LED2_PIN 5

unsigned long lastDebugPrintTime;
int lastLoggedRotation;

void setup() {
  // можно не делать, так как пины по умолчанию в состоянии INPUT
  pinMode(CONTROL_PIN, INPUT);
  // тоже необязательно, критично для digitalWrite, но не analog
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial) {
  }
}

void loop() {
  int rotation, brightness;

  rotation = analogRead(CONTROL_PIN);

  brightness = rotation / 4;
  int brightness2 = brightness / 8;

  analogWrite(LED_PIN, brightness);
  analogWrite(LED2_PIN, brightness2);

  unsigned long time = millis();
  if (time - lastDebugPrintTime > 10000
    || abs(lastLoggedRotation - rotation) > 2
  ) {
    Serial.print(F("Current analog value is: "));
    Serial.print(rotation);
    Serial.print(F(", logged at "));
    Serial.println(time);
    lastDebugPrintTime = time;
    lastLoggedRotation = rotation;
  }
}
