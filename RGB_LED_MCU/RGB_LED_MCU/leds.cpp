/*

Lib for controlling LEDs. LEDs can be driven via two methods. software PWM and interrupt based. The color pallet is meant for writing the brightness and intensity via software interpolation. 
*/
enum ledColor {Red=0, Green=1, Blue=2, White=3};
const uint8_t palletDepth = 10;
const uint8_t numberOfColors = 4;
bool currentPalletMatrix[numberOfColors][palletDepth];
uint8_t currentPalletIndex = 0;


void setColorIntensity(uint8_t colorPalletIndex,uint8_t intensity){
	//colorFillIncrement = (double) palletDepth/((redPercent/100.0)*((double)palletDepth));
	uint8_t colorFillIncrement = 100/intensity; //simplified
	
	if (colorFillIncrement > 0){
		for (uint8_t palletIndex = 0; palletIndex < palletDepth; palletIndex++){
			if(palletIndex % colorFillIncrement == 0 ){
				currentPalletMatrix[colorPalletIndex][palletIndex] = true;
			}
			else{
				currentPalletMatrix[colorPalletIndex][palletIndex] = false;
			}
		}
	}
	else{
		for (uint8_t palletIndex = 0; palletIndex < palletDepth; palletIndex++){ // blank out color pallet
			currentPalletMatrix[Red][palletIndex] = false;
		}
	}
}

void populateColorPallet(uint8_t redPercent, uint8_t greenPercent, uint8_t bluePercent, uint8_t whitePercent){
	setColorIntensity(Red, redPercent);
	setColorIntensity(Green, greenPercent);
	setColorIntensity(Blue, bluePercent);
	setColorIntensity(White, whitePercent);
}

void rgbwWrite(bool red, bool green, bool blue, bool white){
	//PORTA - PORTA7 PORTA6 PORTA5 PORTA4 PORTA3 PORTA2 PORTA1 PORTA0
	uint8_t writeMask = PORTA &  ~(PORTA_OUTPUT_MASK); // copy PORTA masking out the LEDs
	
	writeMask |= red<<PORTA0;      //write red value into output register
	writeMask |= green<<PORTA1;   //write green value into output register
	writeMask |= blue<<PORTA2;     //write blue value into output register	
	writeMask |= white<<PORTA3;    //write white value into output register
	
	PORTA = writeMask; //update PORTA
}

void writeNextPalletIndex(){
	rgbwWrite(currentPalletMatrix[Red][currentPalletIndex],currentPalletMatrix[Green][currentPalletIndex],currentPalletMatrix[Blue][currentPalletIndex],currentPalletMatrix[White][currentPalletIndex]);	
	
	if(currentPalletIndex >= palletDepth-1)
	{
		currentPalletIndex = 0;
	}
	else{
		currentPalletIndex++;  	
	}
}

void cycleLeft(){
	//update pointer to matrix?
}

void cycleRight(){
	
}