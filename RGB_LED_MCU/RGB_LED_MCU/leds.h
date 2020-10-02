#ifndef _LEDS_
#define _LEDS_
#include "leds.cpp"

void wait(uint16_t);
void cycleLeft(void);
void cycleRight(void);
void outputPattern(void);
void updateTickSpeed(uint8_t);
void rgbwWrite(bool, bool, bool, bool);
void ledSoftPWM(uint16_t,uint16_t,uint16_t,uint16_t);
#endif

