#ifndef _MCU_
#define _MCU_
#include "mcu.cpp"
//sleep mode
//remember last setting after sleep? (2 second hold?) after power outage select subtle one
void clearTimer1(void);
void initMcu(void);
void setupADC(void);

uint16_t getKnobAnalogValue(void);
uint8_t getDigitalInputs(void);
void rgbwWrite(bool, bool, bool, bool);
void rgbwClear(void);
void setDutyCycle(double);
double convertAnalogToPercentage(uint16_t);
#endif

