/*
  Скетч создан на основе FASTSPI2 EFFECTS EXAMPLES автора teldredge (www.funkboxing.com)
  А также вот этой статьи https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#cylon
  Доработан, переведён и разбит на файлы 2017 AlexGyver https://github.com/AlexGyver/WS2812_FX
  Фикс потребления памяти из https://github.com/aleksandrbeljakov/WS2812_FX
*/

#include "FastLED.h"          // библиотека для работы с лентой
#include <avr/interrupt.h>
#include <EEPROM.h>

#define LED_COUNT 226          // число светодиодов в кольце/ленте
#define LED_DT 14            // пин, куда подключен DIN ленты

#define EB_NO_FOR
#define EB_DEB_TIME 0
#define EB_HOLD_TIME 2000  // таймаут удержания (кнопка)
#include <EncButton.h>


#undef DEBUG

#define LED_Y 6
#define LED_R 5
#define LED_G 8

//pcint18, 19 - PCINT2
#define ENC_S1 2
#define ENC_S2 3
#define ENC_VCC 4
#define ENC_GND 7
// pcint1
#define ENC_BTN 9

#define MIN_DELAY 0
#define MAX_DELAY 150
#define MIN_BRIGHTNESS 1
#define MAX_BRIGHTNESS 255

#define EEPROM_BRIGHTNESS 0
#define EEPROM_SPEED 4
#define EEPROM_FX 8
#define DEFAULT_TIMEOUT 30

EncButtonT<ENC_S1, ENC_S2, ENC_BTN> eb;

enum Switch {
  BRIGHTNESS,
  SPEED,
  FX,
  IDLE
};
const uint8_t status_leds[] = { LED_G, LED_R, LED_Y };

volatile Switch mode;
volatile bool selected;
volatile uint8_t timeout = 0;
volatile uint8_t pauseBlink = 0;


byte fav_modes[] = { 2, 30, 37, 3, 23, 22, 14, 4, 35, 43, 33, 42, 17, 29, 39, 7, 5, 6, 34, 11, 13, 28, 25, 38, 40, 41, 10, 18, 21};  // список "любимых" режимов

int max_bright;  // максимальная яркость (0 - 255)
int ledMode;
int led_fx = 0;

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(LED_COUNT / 2);
int EVENODD = LED_COUNT % 2;
struct CRGB leds[LED_COUNT];
uint8_t ledsX[LED_COUNT][3]; //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)

volatile int thisdelay = 20; //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int thisindex = 0;

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ihue = 0;                //-HUE (0-255)
int ibright = 0;             //-BRIGHTNESS (0-255)
int isat = 0;                //-SATURATION (0-255)
bool bouncedirection = false;//-SWITCH FOR COLOR BOUNCE (0-1)

volatile boolean changeFlag;
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------

void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}

void one_color_allHSV(int ahue) {    //-SET ALL LEDS TO ONE COLOR (HSV)
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i] = CHSV(ahue, thissat, 255);
  }
}

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);

  pinMode(ENC_VCC, OUTPUT);
  pinMode(ENC_GND, OUTPUT);
  pinMode(ENC_BTN, INPUT_PULLUP);

  digitalWrite(ENC_VCC, HIGH);

  noInterrupts();

  // encoder interrupts
  PCMSK2 = (1 << PCINT19) | (1 << PCINT18);
  PCMSK0 = (1 << PCINT1);
  PCICR = (1 << PCIE2) | (1 << PCIE0);
  eb.setEncISR(true);
  eb.attach(handler);

  // configure timer for mode blinker
  TCCR1A = 0;
  // CTC (Clear Timer on Compare Match) и предделитель 256
  TCCR1B = (1 << WGM12) | (1 << CS12);
  // для результата в 2 Гц
  OCR1A = F_CPU / 256 / 2;
  TIMSK1 |= (1 << OCIE1A);

  interrupts();

  // restore settings
  load_settings();
#ifdef DEBUG
  Serial.println(F("setup finished"));
#endif

#ifdef DEBUG
  handler();  // log current state
#endif

  FastLED.setBrightness(max_bright);  // ограничить максимальную яркость
  FastLED.addLeds<WS2813, LED_DT, RGB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);          // погасить все светодиоды
  FastLED.show();                     // отослать команду
}


void change_mode(int newmode) {
  thissat = 255;
  switch (newmode) {
    case 0: one_color_all(0, 0, 0); FastLED.show(); break; //---ALL OFF
    case 1: one_color_all(255, 255, 255); FastLED.show(); break; //---ALL ON
    case 2: thisdelay = 20; break;                      //---STRIP RAINBOW FADE
    case 3: thisdelay = 20; thisstep = 10; break;       //---RAINBOW LOOP
    case 4: thisdelay = 20; break;                      //---RANDOM BURST
    case 5: thisdelay = 20; thishue = 0; break;         //---CYLON v1
    case 6: thisdelay = 40; thishue = 0; break;         //---CYLON v2
    case 7: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SINGLE
    case 10: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR BRIGHTNESS
    case 11: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR SATURATION
    case 13: thisdelay = 100; break;                    //---CELL AUTO - RULE 30 (RED)
    case 14: thisdelay = 40; break;                     //---MARCH RANDOM COLORS
    //---PLACEHOLDER FOR COLOR LOOP VAR DELAY VARS
    case 21: thisdelay = 100; thishue = 180; break;     //---QUADRATIC BRIGHTNESS CURVE
    //---PLACEHOLDER FOR FLAME VARS
    case 23: thisdelay = 50; thisstep = 15; break;      //---VERITCAL RAINBOW
    case 25: thisdelay = 35; break;                     //---RANDOM COLOR POP
    case 28: thisdelay = 100; thishue = 0; break;       //---KITT
    case 29: thisdelay = 50; thishue = 95; break;       //---MATRIX RAIN
    case 30: thisdelay = 5; break;                      //---NEW RAINBOW LOOP
    case 33: thisdelay = 50; break;                     // colorWipe
    case 34: thisdelay = 50; break;                     // CylonBounce
    case 35: thisdelay = 15; break;                     // Fire
    case 37: thisdelay = 20; break;                     // rainbowCycle
    case 38: thisdelay = 10; break;                     // rainbowTwinkle
    case 39: thisdelay = 50; break;                     // RunningLights
    case 40: thisdelay = 0; break;                      // Sparkle
    case 41: thisdelay = 20; break;                     // SnowSparkle
    case 42: thisdelay = 50; break;                     // theaterChase
    case 43: thisdelay = 50; break;                     // theaterChaseRainbow
  }
  bouncedirection = 0;
  one_color_all(0, 0, 0);
  ledMode = newmode;
}

void loop() {
  eb.tick();
  switch (ledMode) {
    case 999: break;                           // пазуа
    case  2: rainbow_fade(); break;            // плавная смена цветов всей ленты
    case  3: rainbow_loop(); break;            // крутящаяся радуга
    case  4: random_burst(); break;            // случайная смена цветов
    case  5: color_bounce(); break;            // бегающий светодиод
    case  6: color_bounceFADE(); break;        // бегающий паровозик светодиодов
    case  7: ems_lightsONE(); break;           // вращаются красный и синий
    case 10: pulse_one_color_all(); break;     // пульсация одним цветом
    case 11: pulse_one_color_all_rev(); break; // пульсация со сменой цвето
    case 13: rule30_orange(); break;           // безумие красных светодиодов
    case 14: random_march(thisdelay); break;   // безумие случайных цветов
    case 17: color_loop_vardelay(); break;     // красный светодиод бегает по кругу
    case 18: white_temps(); break;             // бело синий градиент (?)
    case 21: quad_bright_curve(); break;       // полумесяц
    case 22: flame(); break;                   // эффект пламени
    case 23: rainbow_vertical(); break;        // радуга в вертикаьной плоскости (кольцо)
    case 25: random_color_pop(); break;        // безумие случайных вспышек
    case 28: kitt(); break;                    // случайные вспышки красного в вертикаьной плоскости
    case 29: matrix(); break;                  // зелёненькие бегают по кругу случайно
    case 30: new_rainbow_loop(); break;        // крутая плавная вращающаяся радуга
    case 33:
      colorWipe(0x00, 0xff, 0x00, thisdelay);
      colorWipe(0x00, 0x00, 0x00, thisdelay);
      break;                                                    // плавное заполнение цветом
    case 34: CylonBounce(0xff, 0, 0, 4, 10, thisdelay); break;  // бегающие светодиоды
    case 35: Fire(55, 120, thisdelay); break;                                       // линейный огонь
    case 37: rainbowCycle(thisdelay); break;                                        // очень плавная вращающаяся радуга
    case 38: TwinkleRandom(20, thisdelay, 1); break;                                // случайные разноцветные включения (1 - танцуют все, 0 - случайный 1 диод)
    case 39: RunningLights(0xff, 0xff, 0x00, thisdelay); break;                     // бегущие огни
    case 40: Sparkle(0xff, 0xff, 0xff, thisdelay); break;                           // случайные вспышки белого цвета
    case 41: SnowSparkle(0x10, 0x10, 0x10, thisdelay, random(100, 1000)); break;    // случайные вспышки белого цвета на белом фоне
    case 42: theaterChase(0xff, 0, 0, thisdelay); break;                            // бегущие каждые 3 (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
    case 43: theaterChaseRainbow(thisdelay); break;                                 // бегущие каждые 3 радуга (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
  }
  changeFlag = false;
}

ISR(PCINT2_vect) {
  eb.tickISR();
}

ISR(PCINT0_vect) {
  eb.tickISR();
}

ISR(TIMER1_COMPA_vect) {
  if (timeout) {
    timeout--;
  }
  if (pauseBlink) {
    pauseBlink--;
  }
  if (mode != Switch::IDLE && !timeout) {
    mode = Switch::IDLE;
    selected = false;
  }
  digitalWrite(LED_BUILTIN,
              thisdelay == MIN_DELAY
                || max_bright == MIN_BRIGHTNESS
                || thisdelay == MAX_DELAY
                || max_bright == MAX_BRIGHTNESS);
  if (pauseBlink) {
    return;
  }
  for (int i = 0; i < sizeof(status_leds) / sizeof(status_leds[0]); ++i) {
    uint8_t pin = status_leds[i];
    bool newValue = i == mode ? selected
                                  ? 1
                                  : !digitalRead(pin)
                              : 0;
    digitalWrite(status_leds[i], newValue);
  }
}

void nextMode() {
  if (mode >= Switch::FX) {
    mode = Switch::BRIGHTNESS;
  } else {
    mode = static_cast<Switch>(static_cast<int>(mode) + 1);
  }
}

void prevMode() {
  if (mode == Switch::BRIGHTNESS) {
    mode = Switch::FX;
  } else {
    mode = static_cast<Switch>(static_cast<int>(mode) - 1);
  }
}

void constrain_and_apply_fx() {
  if (led_fx < 0) {
    led_fx = sizeof(fav_modes) - 1;
  } else if (led_fx >= sizeof(fav_modes)) {
    led_fx = 0;
  }
  change_mode(fav_modes[led_fx]);
}

void load_settings() {
  EEPROM.get(EEPROM_BRIGHTNESS, max_bright);
  EEPROM.get(EEPROM_SPEED, thisdelay);
  EEPROM.get(EEPROM_FX, led_fx);
#ifdef DEBUG
  Serial.print(F("Loaded from eeprom: brightness "));
  Serial.print(max_bright);
  Serial.print(F(", delay "));
  Serial.print(thisdelay);
  Serial.print(F(", fx_num "));
  Serial.println(led_fx);
#endif
  max_bright = constrain(max_bright, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  thisdelay = constrain(thisdelay, MIN_DELAY, MAX_DELAY);

  constrain_and_apply_fx();
}

void save_settings() {
  EEPROM.put(EEPROM_BRIGHTNESS, max_bright);
  EEPROM.put(EEPROM_SPEED, thisdelay);
  EEPROM.put(EEPROM_FX, led_fx);
#ifdef DEBUG
  Serial.println(F("Saved settings to EEPROM"));
#endif
}

void handler() {
#ifdef DEBUG
  Serial.print(F("callback: "));
#endif
  switch (eb.action()) {
    case EB_HOLD:
#ifdef DEBUG
      Serial.println(F("hold"));
#endif
      save_settings();
      pauseBlink = 4;
      for (int i = 0; i < sizeof(status_leds) / sizeof(status_leds[0]); ++i) {
        digitalWrite(status_leds[i], HIGH);
      }
    case EB_CLICK:
      if (mode == Switch::IDLE) {
        nextMode();
      } else {
        selected = !selected;
      }
      timeout = DEFAULT_TIMEOUT;
#ifdef DEBUG
      Serial.println(F("click"));
#endif
      break;
    case EB_TURN:
      timeout = DEFAULT_TIMEOUT;
#ifdef DEBUG
      Serial.print(F("turn "));
      Serial.print(eb.dir());
      Serial.print(' ');
      Serial.print(eb.fast());
      Serial.print(' ');
      Serial.println(eb.pressing());
#endif
      if (selected) {
        switch (mode) {
          case Switch::BRIGHTNESS:
            max_bright = constrain(
              max_bright + eb.dir() * (eb.fast() ? 5 : 1),
              MIN_BRIGHTNESS,
              MAX_BRIGHTNESS);
            FastLED.setBrightness(max_bright);
            break;
          case Switch::SPEED:
            thisdelay = constrain(
              thisdelay - eb.dir() * (eb.fast() ? 3 : 1),
              MIN_DELAY,
              MAX_DELAY);
            break;
          case Switch::FX:
            led_fx = led_fx + eb.dir();
            changeFlag = true;
            constrain_and_apply_fx();
            changeFlag = true;
            break;
          case Switch::IDLE:
            nextMode();
            break;
        }
      } else {
        if (eb.left()) {
          nextMode();
#ifdef DEBUG
          Serial.println(F("left "));
#endif
        } else if (eb.right()) {
          prevMode();
#ifdef DEBUG
          Serial.println(F("right "));
#endif
        }
#ifdef DEBUG
        Serial.print(F("new mode is "));
        Serial.println(mode);
#endif
      }
      break;
      case EB_RELEASE:
#ifdef DEBUG
      Serial.println("release");
#endif
      pauseBlink = 0;
      break;
#ifdef DEBUG
    case EB_PRESS:
      Serial.println("press");
      break;
    case EB_STEP:
      Serial.println("step");
      break;
    case EB_CLICKS:
      Serial.print("clicks ");
      Serial.println(eb.getClicks());
      break;
    case EB_REL_HOLD:
      Serial.println("release hold");
      break;
    case EB_REL_HOLD_C:
      Serial.print("release hold clicks ");
      Serial.println(eb.getClicks());
      break;
    case EB_REL_STEP:
      Serial.println("release step");
      break;
    case EB_REL_STEP_C:
      Serial.print("release step clicks ");
      Serial.println(eb.getClicks());
      break;
#endif
  }
#ifdef DEBUG
  Serial.print(F("  mode "));
  Serial.print(mode);
  Serial.print(F(", selected "));
  Serial.print(selected);
  Serial.print(F(", timeout "));
  Serial.print(timeout);
  Serial.print(F(", led_fx "));
  Serial.print(led_fx);
  Serial.print(" / ");
  Serial.print(ledMode);
  Serial.print(F(", brightness "));
  Serial.print(max_bright);
  Serial.print(F(", delay "));
  Serial.println(thisdelay);
#endif
}
