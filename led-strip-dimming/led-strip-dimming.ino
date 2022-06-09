#include <ButtonDebounce.h>
#include <GyverPWM.h>

#define CONTROL_PIN 9
#define BTN_UP 11
#define BTN_DOWN 12

ButtonDebounce upButton(BTN_UP, 30);
ButtonDebounce downButton(BTN_DOWN, 30);
int brightness = 0;

void setup(){
    pinMode(CONTROL_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(3000);
    digitalWrite(LED_BUILTIN, LOW);

    PWM_TMR1_8BIT();
    PWM_mode(CONTROL_PIN, 0);
    PWM_prescaler(CONTROL_PIN, 1);

    upButton.setCallback([](const int state) {
        if (state == 1 && brightness < 255) {
            brightness++;
        }
        updateLed();
    });

    downButton.setCallback([](const int state) {
        if (state == 1 && brightness > 0) {
            brightness--;
        }
        updateLed();
    });
}

void updateLed() {
    digitalWrite(LED_BUILTIN, brightness == 255);
}

void loop() {
    upButton.update();
    downButton.update();
    analogWrite(CONTROL_PIN, brightness);
}
