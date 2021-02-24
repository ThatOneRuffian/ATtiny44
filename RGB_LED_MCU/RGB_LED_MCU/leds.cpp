void rgbwWrite(bool red, bool green, bool blue, bool white){
	//PORTA - PORTA7 PORTA6 PORTA5 PORTA4 PORTA3 PORTA2 PORTA1 PORTA0
	uint8_t writeMask = PORTA &  ~(PORTA_OUTPUT_MASK); // copy PORTA masking out the LEDs
	
	writeMask |= red<<PORTA0;      //write red value into output register
	writeMask |= blue<<PORTA1;     //write blue value into output register
	writeMask |= green <<PORTA2;   //write green value into output register
	writeMask |= white<<PORTA3;    //write white value into output register
	
	PORTA = writeMask; //update PORTA
}

void cycleLeft(){
	
	//update pointer to matrix?
}

void cycleRight(){
	
}