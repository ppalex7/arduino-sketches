//------------------------LED EFFECT FUNCTIONS------------------------

void rainbow_fade() {                         //-m2-FADE ALL LEDS THROUGH HSV RAINBOW
  ihue++;
  if (ihue > 255) {
    ihue = 0;
  }
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(ihue, thissat, 255);
  }
  LEDS.show();
  delay(thisdelay);
}

void rainbow_loop() {                        //-m3-LOOP HSV RAINBOW
  idex++;
  ihue = ihue + thisstep;
  if (idex >= LED_COUNT) {
    idex = 0;
  }
  if (ihue > 255) {
    ihue = 0;
  }
  leds[idex] = CHSV(ihue, thissat, 255);
  LEDS.show();
  delay(thisdelay);
}

void random_burst() {                         //-m4-RANDOM INDEX/COLOR
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  leds[idex] = CHSV(ihue, thissat, 255);
  LEDS.show();
  delay(thisdelay);
}

void color_bounce() {                        //-m5-BOUNCE COLOR (SINGLE LED)
  if (!bouncedirection) {
    idex = idex + 1;
    if (idex == LED_COUNT) {
      bouncedirection = true;
      idex = idex - 1;
    }
  }
  if (bouncedirection) {
    idex = idex - 1;
    if (idex == 0) {
      bouncedirection = false;
    }
  }
  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == idex) {
      leds[i] = CHSV(thishue, thissat, 255);
    }
    else {
      leds[i] = CHSV(0, 0, 0);
    }
  }
  LEDS.show();
  delay(thisdelay);
}

void ems_lightsONE() {                    //-m7-EMERGENCY LIGHTS (TWO COLOR SINGLE LED)
  idex++;
  if (idex >= LED_COUNT) {
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (thishue + 160) % 255;
  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == idexR) {
      leds[i] = CHSV(thishue, thissat, 255);
    }
    else if (i == idexB) {
      leds[i] = CHSV(thathue, thissat, 255);
    }
    else {
      leds[i] = CHSV(0, 0, 0);
    }
  }
  LEDS.show();
  delay(thisdelay);
}

void pulse_one_color_all_rev() {           //-m11-PULSE SATURATION ON ALL LEDS TO ONE COLOR
  if (!bouncedirection) {
    isat++;
    if (isat >= 255) {
      bouncedirection = true;
    }
  }
  if (bouncedirection) {
    isat = isat - 1;
    if (isat <= 1) {
      bouncedirection = false;
    }
  }
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(thishue, isat, 255);
  }
  LEDS.show();
  delay(thisdelay);
}

#define or_g 107

void random_orange() {                       //QUICK 'N DIRTY RANDOMIZE TO GET CELL AUTOMATA STARTED
  int temprand;
  for (int i = 0; i < LED_COUNT; i++ ) {
    temprand = random(0, 100);
    if (temprand > 50) {
      leds[i].r = 255;
      leds[i].g = or_g;
    }
    if (temprand <= 50) {
      leds[i].r = 0;
      leds[i].g = 0;
    }
    leds[i].b = 0; 
    // leds[i].g = 0;
  }
  LEDS.show();
}


void rule30_orange() {                          //-m13-1D CELLULAR AUTOMATA - RULE 30
  if (!bouncedirection) {
    random_orange();
    bouncedirection = true;
  }
  copy_led_array();
  int iCW;
  int iCCW;
  int y = 100;
  for (int i = 0; i < LED_COUNT; i++ ) {
    iCW = adjacent_cw(i);
    iCCW = adjacent_ccw(i);
    if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] > y) {
      leds[i].r = 0;
      leds[i].g = 0;
    }
    if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] <= y) {
      leds[i].r = 0;
      leds[i].g = 0;
    }
    if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] > y) {
      leds[i].r = 0;
      leds[i].g = 0;
    }
    if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] <= y) {
      leds[i].r = 255;
      leds[i].g = or_g;
    }
    if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] > y) {
      leds[i].r = 255;
      leds[i].g = or_g;
    }
    if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] <= y) {
      leds[i].r = 255;
      leds[i].g = or_g;
    }
    if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] > y) {
      leds[i].r = 255;
      leds[i].g = or_g;
    }
    if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] <= y) {
      leds[i].r = 0;
      leds[i].g = 0;
    }
  }
  LEDS.show();
  delay(80);
}

void random_march(int del) {                   //-m14-RANDOM MARCH CCW
  copy_led_array();
  int iCCW;
  leds[0] = CHSV(random(0, 255), 255, 255);
  for (int idex = 1; idex < LED_COUNT ; idex++ ) {
    iCCW = adjacent_ccw(idex);
    leds[idex].r = ledsX[iCCW][0];
    leds[idex].g = ledsX[iCCW][1];
    leds[idex].b = ledsX[iCCW][2];
  }
  LEDS.show();
  delay(del);
}


void color_loop_vardelay() {                    //-m17-COLOR LOOP (SINGLE LED) w/ VARIABLE DELAY
  idex++;
  if (idex > LED_COUNT) {
    idex = 0;
  }
  int di = abs(TOP_INDEX - idex);
  int t = constrain((10 / di) * 10, 10, 500);
  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == idex) {
      leds[i] = CHSV(0, thissat, 255);
    }
    else {
      leds[i].r = 0; leds[i].g = 0; leds[i].b = 0;
    }
  }
  LEDS.show();
  delay(t);
}

void flame() {                                    //-m22-FLAMEISH EFFECT
  int idelay = random(0, 35);
  float hmin = 0.1; float hmax = 45.0;
  float hdif = hmax - hmin;
  int randtemp = random(0, 3);
  float hinc = (hdif / float(TOP_INDEX)) + randtemp;
  int ihue = hmin;
  for (int i = 0; i <= TOP_INDEX; i++ ) {
    ihue = ihue + hinc;
    leds[i] = CHSV(ihue, thissat, 255);
    int ih = horizontal_index(i);
    leds[ih] = CHSV(ihue, thissat, 255);
    leds[TOP_INDEX].r = 255; leds[TOP_INDEX].g = 255; leds[TOP_INDEX].b = 255;
    LEDS.show();
    delay(idelay);
  }
}

void rainbow_vertical() {                        //-m23-RAINBOW 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX) {
    idex = 0;
  }
  ihue = ihue + thisstep;
  if (ihue > 255) {
    ihue = 0;
  }
  int idexA = idex;
  int idexB = horizontal_index(idexA);
  leds[idexA] = CHSV(ihue, thissat, 255);
  leds[idexB] = CHSV(ihue, thissat, 255);
  LEDS.show();
  delay(thisdelay);
}

void random_color_pop() {                         //-m25-RANDOM COLOR POP
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  one_color_all(0, 0, 0);
  leds[idex] = CHSV(ihue, thissat, 255);
  LEDS.show();
  delay(thisdelay);
}

void matrix() {                                   //-m29-ONE LINE MATRIX
  int rand = random(0, 100);
  if (rand > 90) {
    leds[0] = CHSV(thishue, thissat, 255);
  }
  else {
    leds[0] = CHSV(thishue, thissat, 0);
  }
  copy_led_array();
  for (int i = 1; i < LED_COUNT; i++ ) {
    leds[i].r = ledsX[i - 1][0];
    leds[i].g = ledsX[i - 1][1];
    leds[i].b = ledsX[i - 1][2];
  }
  LEDS.show();
  delay(thisdelay);
}

void new_rainbow_loop() {                      //-m88-RAINBOW FADE FROM FAST_SPI2
  ihue -= 1;
  fill_rainbow( leds, LED_COUNT, ihue );
  LEDS.show();
  delay(thisdelay);
}

//---------------------------------линейный огонь-------------------------------------
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[LED_COUNT];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < LED_COUNT; i++) {
    cooldown = random(0, ((Cooling * 10) / LED_COUNT) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = LED_COUNT - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < LED_COUNT; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  FastLED.show();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

//-----------------------------------------------------------------------------
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < LED_COUNT; i++) {
      c = Wheel(((i * 256 / LED_COUNT) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    FastLED.show();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

//-------------------------------TwinkleRandom---------------------------------------
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0, 0, 0);

  for (int i = 0; i < Count; i++) {
    setPixel(random(LED_COUNT), random(0, 255), random(0, 255), random(0, 255));
    FastLED.show();
    delay(SpeedDelay);
    if (OnlyOne) {
      setAll(0, 0, 0);
    }
  }

  delay(SpeedDelay);
}

//-------------------------------RunningLights---------------------------------------
void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;

  for (int i = 0; i < LED_COUNT * 2; i++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < LED_COUNT; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0);
      //float level = sin(i+Position) * 127 + 128;
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*red,
               ((sin(i + Position) * 127 + 128) / 255)*green,
               ((sin(i + Position) * 127 + 128) / 255)*blue);
    }

    FastLED.show();
    delay(WaveDelay);
  }
}

//-------------------------------Sparkle---------------------------------------
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(LED_COUNT);
  setPixel(Pixel, red, green, blue);
  FastLED.show();
  delay(SpeedDelay);
  setPixel(Pixel, 0, 0, 0);
}

//-------------------------------SnowSparkle---------------------------------------
void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red, green, blue);

  int Pixel = random(LED_COUNT);
  setPixel(Pixel, 0xff, 0xff, 0xff);
  FastLED.show();
  delay(SparkleDelay);
  setPixel(Pixel, red, green, blue);
  FastLED.show();
  delay(SpeedDelay);
}

//-------------------------------theaterChaseRainbow---------------------------------------
void theaterChaseRainbow(int SpeedDelay) {
  byte *c;

  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < LED_COUNT; i = i + 3) {
        c = Wheel( (i + j) % 255);
        setPixel(i + q, *c, *(c + 1), *(c + 2)); //turn every third pixel on
      }
      FastLED.show();
      delay(SpeedDelay);
      for (int i = 0; i < LED_COUNT; i = i + 3) {
        setPixel(i + q, 0, 0, 0);    //turn every third pixel off
      }
    }
  }
}
