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

ISR(TIM1_COMPA_vect){ //timer1A interrupt handler
	static bool toggler = true; //default off

	if(toggler){
		
		rgbwWrite(0,0,0,0); // turn LEDs off	
		OCR1A = maxOnTime; //set on time
		toggler = false;
	}
	
	else{
		rgbwWrite(1,1,1,1); // turn LEDs on
		OCR1A = maxOffTime; //set off time
		toggler = true;
	}
}

int main(void){
	
	initMcu(); 

	while(true) {
		setDutyCycle(convertAnalogToPercentage(getKnobAnalogValue()));
		// read switches 
		// set display mode
		// r,g,b,w - single
		// custom colors
		// math'ed color sweeps
		// low power sleep mode?
	}
}


