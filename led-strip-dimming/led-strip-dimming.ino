#define CONTROL_PIN 9

void setup(){
    pinMode(CONTROL_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

int brightness = 0;

void loop() {
    static boolean step;
    step = !step;
    brightness = ++brightness % 256;
    analogWrite(CONTROL_PIN, brightness);
    digitalWrite(LED_BUILTIN, step);
    delay(58);
}
