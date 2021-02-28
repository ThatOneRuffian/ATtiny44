#ifndef _LEDS_
#define _LEDS_
#include "leds.cpp"

void cycleLeft(void);
void cycleRight(void);
void writeNextPalletIndex(void);
void rgbwWrite(bool, bool, bool, bool);
void setColorIntensity(uint8_t, uint8_t);

#endif

