#ifndef _MCU_
#define _MCU_
#include "mcu.cpp"
//remember last setting after sleep?
void clearTimer1(void);
void initMcu(void);
void setupADC(void);
void wait(uint16_t);
void rgbwClear(void);
void setDutyCycle(double);

uint8_t getDigitalInputs(void);
uint16_t getKnobAnalogValue(void);
double convertAnalogToPercentage(uint16_t);

#endif

