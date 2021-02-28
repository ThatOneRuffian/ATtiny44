#define PORTA_OUTPUT_MASK 0x0F		   //LEDs are logic 1
#define PORTA_UNUSED_MASK 0x40		   //unused pins logic 1
#define PORTA_DIGITAL_SW_MASK 0x30     //digital switches are logic 1
#define PORTA_DIMMER_ANALOG_PIN 0x07   //write to MUX to configure pin 7 for ADC inputs

#define PORTB_MASK 0x0F         //All pins in this group are set to output with pull-up resistors set

// PWM config
uint16_t volatile maxOffTime = 0x0FFF;			
uint16_t volatile maxOnTime = 0x00;			
uint16_t const pwmPeriod = 0x0FFF; //max on+off time for PWM  total frequency ~239Hz

void clearTimer1(){
	TCNT1 = 0x00; //reset timer1 counter to zero
}

void setupADC(){
	//configure ADC parameters
	ADMUX &= ~(0<<REFS0); //set ADC reference voltage to Vcc. Disable AREF on PA0. Make sure these bits are cleared
	ADMUX &= ~(0<<REFS1);
	ADMUX |= PORTA_DIMMER_ANALOG_PIN;	    //configure register to use ADC7
	DIDR0 |= 1<<ADC7D;  //disable digital on pot pin to reduce noise

	ADCSRA |= 1<<ADPS0;   //set pre-scaler division factor to a division factor of 32 or 250kHz assuming that we're running at 8MHz
	ADCSRA |= 1<<ADPS2;

	//ADCSRB – ADC Control and Status Register B
	ADCSRB |= 1<<ADLAR;	//keep ADC 10-bit register data left shifted
	
	//ADCSRA – ADC Control and Status Register A
	ADCSRA |= 1<<ADEN;  //enable ADC	
	ADCSRA |= 1<<ADSC;  //start ADC conversion - first conversion
	
	//------------setup ADC timer interrupts------------
	//setup timer1 (16-bit) and counter compare match interrupts
	//for led brightness PWM
	TCCR1B |= 1<<CS10;  //pre-scaling of 1x
	TCCR1A |= 1<<WGM10; //set to fast-PWM mode with OCR1A as TOP
	TCCR1A |= 1<<WGM11;
	TCCR1B |= 1<<WGM12;
	TCCR1B |= 1<<WGM13;
	TIMSK1 |= 1<<ICIE1;  //enable timer interrupts on timer 1
	TIMSK1 |= 1<<OCIE1A; //enable timer A interrupts on output compare
	OCR1A = 0xFFFF; //set analog sample frequency
}

void initMcu(){
	//setup I/O ports - configure for ADC input
	DDRA = PORTA_OUTPUT_MASK;	     //set pins 0-3 on PORTA to output for LEDs. Pins 4-5 as inputs with interrupts
	PORTA = PORTA_DIGITAL_SW_MASK;	 //configure PORTA pull-up resistors on the digital switches. disable pull-ups on ADC and write other outputs low.
	PORTA |= PORTA_UNUSED_MASK;      //configure unused pins on PORTA to turn on pull-up resistor
	
	//configure unused pins on PORTB
	DDRB = PORTB_MASK;    //set all pins on PORTB to output to avoid high-Z
	PORTB |= PORTB_MASK;  //turn on pull-up resistors for unused pins
	
	setupADC();
	
	sei();	//enable global interrupts, I-bit
	
	clearTimer1();
}

void delay_us(uint16_t waitUs){  // wrapper to accept integers
	while (waitUs > 0)
	{
		_delay_us(1);
		waitUs--;
	}
}

void delay_ms(uint16_t waitMs){
	const uint8_t milliToMicro = 10;
	delay_us(waitMs*milliToMicro);
}

void setDutyCycle(double onPercentage){
	const double lowerLimit = 3;
	const double upperLimit = 97;
	
	if(onPercentage >= lowerLimit && onPercentage <= upperLimit){

		maxOnTime = (uint16_t) (((pwmPeriod * onPercentage)*100)/10000); //preserve INTs
		maxOffTime = (uint16_t) pwmPeriod - maxOnTime;
	}
}

double convertAnalogToPercentage(uint16_t analogRead){
	return ((double) analogRead * 100.0)/(65536.0); //reducing steps to convert into percentage
}

uint16_t getKnobAnalogValue(){ //knob that controls the LED PWM input interpolation

	ADCSRA |= 1<<ADSC; //start ADC conversion
	loop_until_bit_is_clear(ADCSRA, ADSC); //ADSC register position will return 0 if there is no conversion in progress
	
	return  convertAnalogToPercentage(ADC);//return high byte of left shifted ADC read converted into on percentage
}

bool getDigitalInputA(uint8_t pinA_index_to_check){
	if(bit_is_set(PINA, pinA_index_to_check)){
		return true;
	}
	else{
		return false;
	}
}
