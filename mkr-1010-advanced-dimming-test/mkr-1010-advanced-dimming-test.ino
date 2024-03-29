#define PIN_L2_L (PIN_A5)   // PA06
#define PIN_L2_R (PIN_A6)   // PA07
#define PIN_L3_W (2u)       // PA10
#define PIN_L3_R (3u)       // PA11
#define PIN_L3_G (4u)       // PB10
#define PIN_L3_B (5u)       // PB11

#define pinToPINFCG(P)      (PORT->Group[ g_APinDescription[P].ulPort ].PINCFG[ g_APinDescription[P].ulPin ])
#define pinToPMUX(P)        (PORT->Group[ g_APinDescription[P].ulPort ].PMUX[ g_APinDescription[P].ulPin >> 1 ])

void setup() {
  // 16.6.2.6 Peripheral Clock Default state (according to datasheet):
    // CLK_PORT_APB — Enabled
    // CLK_TCx_APB — Disabled
    // CLK_TCCx_APB — Disabled

  // 31.5 Timer/Counter for Control Applications. Product dependencies:
  // 31.5.1. In order to use I/O lines of this peripheral, the I/O pins must be configured using the I/O Pin Controller (PORT)
    // 23.5.2 During Reset, all PORT lines are configured as inputs with input buffers, output buffers and pull disabled.
  pinToPINFCG(PIN_L3_W).bit.PMUXEN = 1;
  pinToPMUX(PIN_L3_W).bit.PMUXE = PORT_PMUX_PMUXE_F_Val;

  pinToPINFCG(PIN_L3_R).bit.PMUXEN = 1;
  pinToPMUX(PIN_L3_R).bit.PMUXO = PORT_PMUX_PMUXO_F_Val;

  pinToPINFCG(PIN_L3_G).bit.PMUXEN = 1;
  pinToPMUX(PIN_L3_G).bit.PMUXE = PORT_PMUX_PMUXE_F_Val;

  pinToPINFCG(PIN_L3_B).bit.PMUXEN = 1;
  pinToPMUX(PIN_L3_B).bit.PMUXO = PORT_PMUX_PMUXO_F_Val;

  pinToPINFCG(PIN_L2_L).bit.PMUXEN = 1;
  pinToPMUX(PIN_L2_L).bit.PMUXE = PORT_PMUX_PMUXE_E_Val;

  pinToPINFCG(PIN_L2_R).bit.PMUXEN = 1;
  pinToPMUX(PIN_L2_R).bit.PMUXO = PORT_PMUX_PMUXO_E_Val;

  // временно, для дебага
  // вывести конфигурируемый нами GCLK_4 на D6 (PA20)
  pinToPINFCG(6u).bit.PMUXEN = 1;
  pinToPMUX(6u).bit.PMUXE = PORT_PMUX_PMUXE_H_Val;

  // 31.5.3 The TCC bus clocks (CLK_TCCx_APB) can be enabled and disabled in the Power Manager module.
  // The default state of CLK_TCCx_APB can be found in the Peripheral Clock Masking.
  PM->APBCMASK.reg |= PM_APBCMASK_TCC0; // excess: it was already configured by init function in wiring.c
  // A generic clock (GCLK_TCCx) is required to clock the TCC.
  // This clock must be configured and enabled in the generic clock controller before using the TCC.
  // Note that TCC0 and TCC1 share a peripheral clock generator.
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(4) // generic clock generators from 0 to 3 already used in startup.c
    | GCLK_GENDIV_DIV(4)
  ;
  while(GCLK->STATUS.bit.SYNCBUSY);

  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(4)
    | GCLK_GENCTRL_SRC_DFLL48M
    | GCLK_GENCTRL_GENEN
    | GCLK_GENCTRL_IDC
    | GCLK_GENCTRL_OE // for debug
    | GCLK_GENCTRL_RUNSTDBY // for debug
  ;
  while(GCLK->STATUS.bit.SYNCBUSY);
  // in result: 384 kHz by divisor 128 from 48 MHz source

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TCC0_TCC1
    | GCLK_CLKCTRL_GEN(4)
    | GCLK_CLKCTRL_CLKEN
  ;
  while(GCLK->STATUS.bit.SYNCBUSY);
  // The generic clocks (TCLK_TCCx) are asynchronus to the bus clock (CLK_TCCx_APB).
  // Due to this asynchronicity, writing certain registers will require synchronization between the clock domains.
  TCC0->WAVE.bit.WAVEGEN = TCC_WAVE_WAVEGEN_NPWM_Val;
  while(TCC0->SYNCBUSY.bit.WAVE);

  TCC0->PER.bit.PER = (uint16_t) 1000;
  while(TCC0->SYNCBUSY.bit.PER);

  // GREEEN
  TCC0->CC[0].bit.CC = 300;
  while(TCC0->SYNCBUSY.bit.CC0);

  // Blue
  TCC0->CC[1].bit.CC = 400;
  while(TCC0->SYNCBUSY.bit.CC1);

  //white
  TCC0->CC[2].bit.CC = 100;
  while(TCC0->SYNCBUSY.bit.CC2);

  //red
  TCC0->CC[3].bit.CC = 200;
  while(TCC0->SYNCBUSY.bit.CC3);

  TCC0->CTRLA.bit.ENABLE = 1;
  while(TCC0->SYNCBUSY.bit.ENABLE);

  TCC1->WAVE.bit.WAVEGEN = TCC_WAVE_WAVEGEN_NPWM_Val;
  while(TCC1->SYNCBUSY.bit.WAVE);

  TCC1->PER.bit.PER = (uint16_t) 1000;
  while(TCC1->SYNCBUSY.bit.PER);

  TCC1->CC[0].bit.CC = 600;
  while(TCC1->SYNCBUSY.bit.CC0);

  TCC1->CC[1].bit.CC = 700;
  while(TCC1->SYNCBUSY.bit.CC1);

  TCC1->CTRLA.bit.ENABLE = 1;
  while(TCC1->SYNCBUSY.bit.ENABLE);
}

void loop() {
  // put your main code here, to run repeatedly:

}
