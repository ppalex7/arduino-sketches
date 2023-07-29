#define PIN_L3_W (2u)       // PA10
#define PIN_L3_R (3u)       // PA11
#define PIN_L3_G (4u)       // PB10
#define PIN_L3_B (5u)       // PB11


void setup() {
  // 31.5 Timer/Counter for Control Applications. Product dependencies:
  // 31.5.1. In order to use I/O lines of this peripheral, the I/O pins must be configured using the I/O Pin Controller (PORT)
  PORT->Group[ g_APinDescription[ PIN_L3_W ].ulPort ].PINCFG[ g_APinDescription[ PIN_L3_W ].ulPin ].bit.PMUXEN = 1;

  // 31.5.3 The TCC bus clocks (CLK_TCCx_APB) can be enabled and disabled in the Power Manager module.
  // The default state of CLK_TCCx_APB can be found in the Peripheral Clock Masking.
  // A generic clock (GCLK_TCCx) is required to clock the TCC.
  // This clock must be configured and enabled in the generic clock controller before using the TCC.
  // Note that TCC0 and TCC1 share a peripheral clock generator.
  // The generic clocks (TCLK_TCCx) are asynchronus to the bus clock (CLK_TCCx_APB).
  // Due to this asynchronicity, writing certain registers will require synchronization between the clock domains.


}

void loop() {
  // put your main code here, to run repeatedly:

}
