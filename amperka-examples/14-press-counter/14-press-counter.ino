#define DATA_PIN 5
#define LATCH_PIN 4
#define CLOCK_PIN 3
#define BUTTON_PIN 14

int clicks = 0;
boolean buttonPrevState = true;
byte segments[10] = {
  0b01111101,
  0b00100100,
  0b01111010,
  0b01110110,
  0b00100111,
  0b01010111,
  0b01011111,
  0b01100100,
  0b01111111,
  0b01110111,
};

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  if (!buttonPrevState && digitalRead(BUTTON_PIN)) {
    clicks = (clicks + 1) % 10;
  }
  buttonPrevState = digitalRead(BUTTON_PIN);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, segments[clicks]);
  digitalWrite(LATCH_PIN, HIGH);
}
