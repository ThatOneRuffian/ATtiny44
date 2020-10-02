void wait(uint16_t waitUs){
	while (waitUs > 0)
	{
		_delay_us(1);
		waitUs--;
	}
}


void ledSoftPWM(uint16_t redOnTime,uint16_t greenOnTime, uint16_t blueOnTime, uint16_t whiteOnTime){
	
	if(redOnTime > 0){ //check of on time is == 0 to skip colors not in sequence 
		rgbwWrite(1,0,0,0);
		wait(redOnTime);
		rgbwClear();
	}
	
	if(greenOnTime > 0){ //check of on time is == 0 to skip colors not in sequence
		rgbwWrite(0,1,0,0);
		wait(greenOnTime);
		rgbwClear();
	}
	
	if(blueOnTime > 0){ //check of on time is == 0 to skip colors not in sequence
		rgbwWrite(0,0,1,0);
		wait(blueOnTime);
		rgbwClear();
	}
	
	if(whiteOnTime > 0){ //check of on time is == 0 to skip colors not in sequence
		rgbwWrite(0,0,0,1);
		wait(whiteOnTime);
		rgbwClear();
	}	
}

void cycleLeft(){
	
	//update pointer to matrix?
}

void cycleRight(){
	
}

void updateTickSpeed(uint8_t analogValue){

}

