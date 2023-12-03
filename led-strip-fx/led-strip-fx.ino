/*
  Copy from https://github.com/AlexGyver/WS2812_FX
  Скетч создан на основе FASTSPI2 EFFECTS EXAMPLES автора teldredge (www.funkboxing.com)
  А также вот этой статьи https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#cylon
  Доработан, переведён и разбит на файлы 2017 AlexGyver
  Отправляем в монитор порта номер режима, он активируется
*/

#include "FastLED.h"          // библиотека для работы с лентой

#define LED_COUNT 228          // число светодиодов в кольце/ленте
#define LED_DT 3             // пин, куда подключен DIN ленты

int max_bright = 51;         // максимальная яркость (0 - 255)
// int max_bright = 255;
int ledMode = 3;
/*
  Стартовый режим
  0 - все выключены
  1 - все включены
  3 - кольцевая радуга
*/

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(LED_COUNT / 2);
int EVENODD = LED_COUNT % 2;
struct CRGB leds[LED_COUNT];
int ledsX[LED_COUNT][3];     //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)

int thisdelay = 20;          //-FX LOOPS DELAY VAR was 20
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int thisindex = 0;
int thisRED = 0;
int thisGRN = 0;
int thisBLU = 0;

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ihue = 0;                //-HUE (0-255)
int ibright = 0;             //-BRIGHTNESS (0-255)
int isat = 0;                //-SATURATION (0-255)
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
int lcount = 0;              //-ANOTHER COUNTING VAR
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
  Serial.begin(9600);              // открыть порт для связи
  LEDS.setBrightness(max_bright);  // ограничить максимальную яркость

  // LEDS.addLeds<WS2813, LED_DT, GRB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  LEDS.addLeds<WS2813, LED_DT, RGB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);          // погасить все светодиоды
  LEDS.show();                     // отослать команду
}


void change_mode(int newmode) {
  thissat = 255;
  Serial.print(newmode);
  Serial.println(" will be enabled");
  switch (newmode) {
    case 0: one_color_all(0, 0, 0); LEDS.show(); break; //---ALL OFF
    case 1: one_color_all(255, 255, 255); LEDS.show(); break; //---ALL ON
    case 2: thisdelay = 20; break;                      //---STRIP RAINBOW FADE
    case 3: thisdelay = 20; thisstep = 10; break;       //---RAINBOW LOOP
    case 4: thisdelay = 20; break;                      //---RANDOM BURST
    case 5: thisdelay = 20; thishue = 0; break;         //---CYLON v1
    case 6: thisdelay = 40; thishue = 0; break;         //---CYLON v2
    case 7: thisdelay = 40; thishue = 0; break;         //---POLICE LIGHTS SINGLE
    case 9: thishue = 160; thissat = 50; break;         //---STRIP FLICKER
    case 10: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR BRIGHTNESS
    case 11: thisdelay = 15; thishue = 0; break;        //---PULSE COLOR SATURATION
    case 12: thisdelay = 60; thishue = 180; break;      //---VERTICAL SOMETHING
    case 13: thisdelay = 100; break;                    //---CELL AUTO - RULE 30 (RED)
    case 14: thisdelay = 40; break;                     //---MARCH RANDOM COLORS
    //---PLACEHOLDER FOR COLOR LOOP VAR DELAY VARS
    //---PLACEHOLDER FOR FLAME VARS
    case 23: thisdelay = 50; thisstep = 15; break;      //---VERITCAL RAINBOW
    case 25: thisdelay = 35; break;                     //---RANDOM COLOR POP
    case 29: thisdelay = 50; thishue = 95; break;       //---MATRIX RAIN
    case 30: thisdelay = 5; break;                      //---NEW RAINBOW LOOP
    case 35: thisdelay = 15; break;                     // Fire
    case 37: thisdelay = 20; break;                     // rainbowCycle
    case 38: thisdelay = 10; break;                     // rainbowTwinkle
    case 39: thisdelay = 50; break;                     // RunningLights
    case 40: thisdelay = 0; break;                      // Sparkle
    case 41: thisdelay = 20; break;                     // SnowSparkle
    case 43: thisdelay = 50; break;                     // theaterChaseRainbow

    case 101: one_color_all(255, 0, 0); LEDS.show(); break; //---ALL RED
    case 102: one_color_all(0, 255, 0); LEDS.show(); break; //---ALL GREEN
    case 103: one_color_all(0, 0, 255); LEDS.show(); break; //---ALL BLUE
    case 104: one_color_all(255, 255, 0); LEDS.show(); break; //---ALL COLOR X
    case 105: one_color_all(0, 255, 255); LEDS.show(); break; //---ALL COLOR Y
    case 106: one_color_all(255, 0, 255); LEDS.show(); break; //---ALL COLOR Z
  }
  Serial.println("afterSwitch");
  bouncedirection = 0;
  one_color_all(0, 0, 0);
  Serial.println("afterFill");
  ledMode = newmode;
}

void loop() {

  if (Serial.available() > 0) {     // если что то прислали
    ledMode = Serial.parseInt();    // парсим в тип данных int
    change_mode(ledMode);           // меняем режим через change_mode (там для каждого режима стоят цвета и задержки)
  }
  switch (ledMode) {
    case 999: break;                           // пазуа
    case  2: rainbow_fade(); break;            // плавная смена цветов всей ленты
    case  3: rainbow_loop(); break;            // крутящаяся радуга
    case  4: random_burst(); break;            // случайная смена цветов
    case  5: color_bounce(); break;            // бегающий светодиод
    case  6: color_bounceFADE(); break;        // бегающий паровозик светодиодов
    case  7: ems_lightsONE(); break;           // вращаются красный и синий
    case 13: rule30_orange(); break;           // безумие оранжевых светодиодовв
    case  9: flicker(); break;                 // случайный стробоскоп
    case 10: pulse_one_color_all(); break;     // пульсация одним цветом
    case 11: pulse_one_color_all_rev(); break; // пульсация со сменой цветов
    case 12: fade_vertical(); break;           // плавная смена яркости по вертикали (для кольца)
    case 14: random_march(thisdelay); break;            // безумие случайных цветов
    case 17: color_loop_vardelay(); break;     // красный светодиод бегает по кругу
    case 18: white_temps(); break;             // бело синий градиент (?)
    case 22: flame(); break;                   // эффект пламени
    case 23: rainbow_vertical(); break;        // радуга в вертикаьной плоскости (кольцо)
    case 25: random_color_pop(); break;        // безумие случайных вспышек
    case 29: matrix(); break;                  // зелёненькие бегают по кругу случайно
    case 30: new_rainbow_loop(); break;        // крутая плавная вращающаяся радуга
    case 35: Fire(55, 120, thisdelay); break;                                       // линейный огонь
    case 37: rainbowCycle(thisdelay); break;                                        // очень плавная вращающаяся радуга
    case 38: TwinkleRandom(20, thisdelay, 1); break;                                // случайные разноцветные включения (1 - танцуют все, 0 - случайный 1 диод)
    case 39: RunningLights(0xff, 0xff, 0x00, thisdelay); break;                     // бегущие огни
    case 40: Sparkle(0xff, 0xff, 0xff, thisdelay); break;                           // случайные вспышки белого цвета
    case 41: SnowSparkle(0x10, 0x10, 0x10, thisdelay, random(100, 1000)); break;    // случайные вспышки белого цвета на белом фоне
    case 43: theaterChaseRainbow(80); break;                                 // бегущие каждые 3 радуга (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО 3)
case 51: random_march(90); break;            // безумие случайных цветов
case 53: RunningLights(0xff, 0x6b, 0x00, thisdelay); break;                     // бегущие огни
case 54: Sparkle(0xff, 0x6b, 0x00, 0); break;                           // случайные вспышки белого цвета
case 55: Sparkle(0xff, 0x6b, 0x00, 5); break;                           // случайные вспышки белого цвета
case 56: Sparkle(0xff, 0x6b, 0x00, 10); break;                           // случайные вспышки белого цвета
  }
}
