#ifndef _MCU_
#define _MCU_
#include "mcu.cpp"
//remember last setting after sleep?

void initMcu(void);
void setupADC(void);
void delay_us(uint16_t);
void delay_ms(uint16_t);
void rgbwClear(void);
void clearTimer1(void);
void setDutyCycle(double);
uint8_t getDigitalInputs(void);
bool getDigitalInputs2(uint8_t);
uint16_t getKnobAnalogValue(uint8_t);
double convertAnalogToPercentage(uint16_t);

#endif

