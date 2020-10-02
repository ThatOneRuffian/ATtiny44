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
uint16_t analogRead;
bool rgbwBoolMatrix[] = {0,1,1,0}; //need to make something where I can only toggle the ones I need, functions that write to this global var to change colors, patterns, frequncy that they pulse update
void updatePWM(void);
uint8_t getDiff(uint8_t, uint8_t);

ISR(TIM1_COMPA_vect){ //timer1A interrupt
	static bool toggler = true; //default off
	//convertAnalogToPercentage(getKnobAnalogValue())
	//setDutyCycle2(10);
	if(toggler){
		
		rgbwWrite(0,0,0,0); // turn LEDs off	
		OCR1A = maxOnTime; //set on time
		toggler = false;
	}
	
	else{
		rgbwWrite(1,0,0,0); // turn LEDs on
		OCR1A = maxOffTime; //set off time
		toggler = true;
	}
}

// ISR(ADC_vect){
// 	//rgbwWrite(1,1,1,1); // turn LEDs on
// 
// 	//rgbwWrite(1,1,1,1); // turn LEDs on
// 	updatePWM();
// 	//setDutyCycle2(convertAnalogToPercentage(getKnobAnalogValue()));
// }

ISR(TIM0_COMPA_vect){  //compare match on timer0
		updatePWM();
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
	
	while(true)   {
	}

}

void updatePWM(){ // unable to use interrupts - too much flickering
	const uint8_t arrayLength = 10;
	static bool toggler = true; //keep tract of the pointer's direction
	static uint16_t newRead;
	static uint8_t readArray[arrayLength];
	static uint8_t *indexPtr = readArray;
	static uint8_t counter = 0; //count the number of entries made
	static uint16_t z =0;
	
	uint16_t arrayAvg; //need larger value to store - just in case all values were maxed out
	
	newRead = convertAnalogToPercentage(getKnobAnalogValue());
	
	for(uint16_t i = 0; i < sizeof(uint8_t)*arrayLength; i++){ 	// get average of reads
		arrayAvg+=readArray[i]; //add up all values of the array and store into a variable
	}
	
	arrayAvg /= arrayLength; //get current array average
	
	if (true) //if current read is equal to the average
	{
		setDutyCycle2(newRead);
	}
	
	if (counter == arrayLength-1){
		counter = 0; //reset counter
		z = 0;
	}	
}

uint8_t	getDiff(uint8_t a, uint8_t b){ //safe math wrapper
	
	if(a > b){
		return a - b;
	}
	else if (b > a){
		return b - a;		
	}
	else{
		return 0; 
	}
}