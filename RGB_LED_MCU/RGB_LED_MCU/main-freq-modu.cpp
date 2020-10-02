/*
 * RGB_LED_MCU.cpp
 * Code to control an RGBW led. Including controlling the brightness with a potentiometer knob through ADC, cycling through a set of colors, patterns, and a sleep mode. Written for the ATTINY44A
 * Created: 6/25/2020 8:40:24 AM
 * Author : Marcus
 */ 

#define F_CPU 8000000UL		   //tell compiler how quickly the CPU is running, so delay functions can be initialized - must be initialized before including delay.h

#include <avr/io.h>		       //memory mapping
#include <avr/common.h>		   //SREG definition for handling interrupts
#include <util/delay.h>
#include <avr/interrupt.h>
#include "mcu.h"			   //functions for configuring and using the micro controller
#include "leds.h"			   //functions for controlling the LEDs 

//debug functions
uint32_t const ledPluseSpeed[] = {1,10,30,100,200}; // ms
uint8_t ledIntensityMask[4] = {255,255,255,255}; //rgbw softPWM mask
volatile uint16_t analogRead;

ISR(TIM1_COMPA_vect){ //timer1A interrupt
	rgbwWrite(1,1,1,1); // turn LEDs on
}

ISR(TIM1_COMPB_vect){ //timer1B interrupt
	rgbwWrite(0,0,0,0); // turn LEDs off
}

ISR(ADC_vect){  //ADC conversion complete interrupt	
	setDutyCycle(convertAnalogToPercentage(ADC)); //10-bit ADC register internally converted/scaled into 16-bit value
}

/*
ISR(INT4_vect){ //interrupts to handle button pushes
	cycleLeft(); 
}

ISR(INT5_vect){ 
	cycleRight(); 
}*/

int main(void){
	
	initMcu();    

	while (1) 
    {
		//setDutyCycle(25.0);
		//fluff for debugging below
		updateTickSpeed(8);		
		outputPattern();
		cycleLeft();
		
    }
}