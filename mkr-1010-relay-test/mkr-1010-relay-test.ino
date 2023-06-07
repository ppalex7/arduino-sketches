#include "MCP23017.h"
#include "Wire.h"

#define ADDR 0b0100111

// relay connected from GPA1 to GPA6
// using high byte from bits, starting from 1 in GPA0 position
#define INITAL_BITS 0b111111000

MCP23017 MCP(ADDR);

static int bits = INITAL_BITS;

void setup() {
  Wire.begin();

  MCP.begin();

  MCP.pinMode8(0, 0x00);
}

void loop() {
  MCP.write8(0, 0xFF & bits >> 8);

  delay(167);
  bits = bits << 1;
  if (bits == (INITAL_BITS << 7) || bits == (INITAL_BITS << 13)) {
    delay(2000);
  }
  if (bits == (INITAL_BITS << 13)) {
    bits = INITAL_BITS;
  }
}
