#define CONTROL_PIN 9

void setup(){
    pinMode(CONTROL_PIN, OUTPUT);
}

int brightness = 0;

void loop() {
    brightness = (brightness + 3) % 256;
    analogWrite(CONTROL_PIN, brightness);
    delay(10);
}
