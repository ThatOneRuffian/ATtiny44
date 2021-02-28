#ifndef _MCU_
#define _MCU_
#include "mcu.cpp"
//remember last setting after sleep?

void initMcu(void);
void setupADC(void);
void rgbwClear(void);
void clearTimer1(void);
void delay_us(uint16_t);
void delay_ms(uint16_t);
void setDutyCycle(double);
bool getDigitalInputA(uint8_t);
uint16_t getKnobAnalogValue(uint8_t);
double convertAnalogToPercentage(uint16_t);

#endif

