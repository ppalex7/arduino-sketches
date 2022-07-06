#include <ButtonDebounce.h>

#define CONTROL_PIN 9
#define BTN_UP 11
#define BTN_DOWN 12

ButtonDebounce upButton(BTN_UP, 30);
ButtonDebounce downButton(BTN_DOWN, 30);
int brightness = 0;

/*
  Что еще удалось понять: у Counter1, в отличие от Counter0/Counter2 есть Input Capture Unit со своим регистром (ICR1),
    который таймер позволяет использовать в качестве значения TOP. Это позволяет произвольно ставить частоту ШИМ
    (но общую на оба пина), так как регистр OCR1A остается для использования под заполнение (duty).
  Включение/выключение таймера идет через COM1A1 (это видно по изменению TCCR1A с 2 на 130 и обратно). Чтобы не делать
    этого вручную на граничных значениях, использована analogWrite у которой это под капотом. Правда такой варинат не подойдет,
    если ICR1 получится больше 1 байта
 */

void setup(){
    Serial.begin(9600);

    pinMode(CONTROL_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(3000);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.print("Original values, TCCR1A: ");
    Serial.print(TCCR1A);
    Serial.print(" , TCCR1B: ");
    Serial.println(TCCR1B);

    /* TCCR1A – Timer/Counter1 Control Register A
      COM1A1 COM1A0 COM1B1 COM1B0 – – WGM11 WGM10
      Bit 7:6 – COM1A1:0: Compare Output Mode for Channel A
      Bit 5:4 – COM1B1:0: Compare Output Mode for Channel B
        биты 7-4 по умолчанию равны 0: (in FastPWM mode) Normal port operation, OC1A/OC1B disconnected.
      Bit 1:0 – WGM11:0: Waveform Generation Mode.
        Combined with the WGM13:2 bits found in the TCCR1B register, these bits control the counting sequence of the counter,
        the source for maximum (TOP) counter value, and what type of waveform generation to be used, see Table 15-5.
        нам по табличке нужен Mode 14: WGM13-10: 1110, Fast PWM, TOP: ICR1, Update of OCR1x at BOTTOM, TOV1 Flag Set on TOP
    */
    // 0xFC — маска, сохраняющая значения всех бит кроме двух младших (WGM1*) >>> hex(0b00000011 ^ 0xFF)
    TCCR1A = (TCCR1A & 0xFC) | 1 << WGM11;

    /* TCCR1B – Timer/Counter1 Control Register B
      ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
      Bit 5 – Reserved Bit
        For ensuring compatibility with future devices, this bit must be written to zero when TCCR1B is written.
      Bit 4:3 – WGM13:2: Waveform Generation Mode
      Bit 2:0 – CS12:0: Clock Select
        The three clock select bits select the clock source to be used by the Timer/Counter, see
        нам нужен 0b010 — clkIO/8 (from prescaler)
    */
    // 0xC0 — маска, сохраняющая старшие два бита >>> hex(0b00111111 ^ 0xFF)

    TCCR1B = (TCCR1B & 0xC0) | 1 << WGM13 | 1 << WGM12 | 1 << CS11;

    // ставим разрешение счетчика: 12 кГц — частота которую хотим, 8 — prescaler (СS11), ATmega328P 16 MHz -> получаем 165
    /* ставить надо после TCCR1*
      The ICR1 register can only be written when using a waveform generation mode that utilizes the ICR1 register
      for defining the counter’s TOP value. In these cases the waveform generation mode (WGM13:0) bits
      must be set before the TOP value can be written to the ICR1 register.
    */
    ICR1 = (F_CPU / (8 * 12000UL)) - 1;

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

    delay(500);
    Serial.print("Updated values, TCCR1A: ");
    Serial.print(TCCR1A);
    Serial.print(" , TCCR1B: ");
    Serial.println(TCCR1B);
}

void setDuty(int pin, int duty) {
  int val;
  if (duty == 0 || duty == 255) {
    val = duty;
  } else {
    val = map(duty, 0, 255, 0, ICR1);
  }
  analogWrite(pin, val);
}

void updateLed() {
    digitalWrite(LED_BUILTIN, brightness == 255);
    Serial.print("Current values, TCCR1A: ");
    Serial.print(TCCR1A);
    Serial.print(" , TCCR1B: ");
    Serial.print(TCCR1B);
    Serial.print(" , ICR1H: ");
    Serial.print(ICR1H);
    Serial.print(" , ICR1L: ");
    Serial.print(ICR1L);
    Serial.print(", brightness: ");
    Serial.println(brightness);
}

void loop() {
    upButton.update();
    downButton.update();
    setDuty(CONTROL_PIN, brightness);
}
