#define PORTA_OUTPUT_MASK 0x0F		   //LEDs are logic 1
#define PORTA_UNUSED_MASK 0x40		   //unused pins logic 1
#define PORTA_DIGITAL_SW_MASK 0x30     //digital switches are logic 1
#define PORTA_DIMMER_ANALOG_PIN 0x07   //write to MUX to configure pin 7 for ADC inputs

#define PORTB_MASK 0x03         //left RESET (PB3) register unchanged. All other pins in this group are set to output with pull-up resistors set

uint16_t volatile maxOffTime = 0x1FFF;			
uint16_t volatile maxOnTime = 0x00;			
uint16_t const pwmPeriod = 0x0FFF; //max on/off time for PWM  total frequency ~239Hz

void clearTimer1(){
	TCNT1 = 0x00; //reset timer1 counter to zero
}

void setupADC(){
	//configure ADC parameters
	//ADCSRA – ADC Control and Status Register A
	//ADCSRB – ADC Control and Status Register B
	//ACSR - Analog Comparator Control and Status Register
	ADMUX &= ~(0<<REFS0); //set ADC reference voltage to Vcc. Disable AREF on PA0. Make sure these bits are cleared
	ADMUX &= ~(0<<REFS1);
	ADMUX  |= PORTA_DIMMER_ANALOG_PIN;	    //configure register to use ADC7

	//ADCSRA &= ~(1<<ADATE); //disable Auto Trigging
	//ADCSRA &= ~(1<<ADIE);  //disable ADC Conversion complete interrupt  - when i-bit is set in SREG
	
	//ADCSRA &= ~(1<<ADPS0);
	//ADCSRA |= 1<<ADPS1;
	ADCSRA |= 1<<ADPS0;   //set prescaler division factor to a division factor of 32 or 250kHz assuming that we're running at 8MHz
	ADCSRA |= 1<<ADPS2;

	ADCSRB |= 1<<ADLAR;         //keep 10-bit register data left shifted
	//ADCSRB &= ~(0<<ADTS0);	//set ADC to run in free running mode
	//ADCSRB &= ~(0<<ADTS1);
	//ADCSRB &= ~(0<<ADTS2);
	DIDR0 |= 1<<ADC7D; //disable digital on pot pin. Stops analog read from jittering(?)

	ADCSRA |= 1<<ADEN;  //enable ADC
	
	//ADCSRA |= 1<<ADIE;  //enable ADC interrupt
	//ADCSRA |= 1<<ADATE; //enable auto trigger
	

	
	//ADCSRB |= 1<<ADTS0; // set to interrupt on compare match B on timer1
	//ADCSRB |= 1<<ADTS2;
	//ADCSRB |= 1<<ADTS0; // set ADC interrupt on compare match 0A
	//ADCSRB |= 1<<ADTS1;
	ADCSRA |= 1<<ADSC;  //start ADC conversion - first conversion
}

void initMcu(){
	//setup I/O ports - configure for ADC input
	DDRA = PORTA_OUTPUT_MASK;			 //set pins 0-3 on PORTA to output for LEDs. Pins 4-5 as inputs with interrupts
	PORTA = PORTA_DIGITAL_SW_MASK;	 //configure PORTA pull-up resistors on the digital switches. disable pull-ups on ADC and write other outputs low.
	PORTA |= PORTA_UNUSED_MASK;  //configure unused pins on PORTA to turn on pull-up resistor
	
	//configure unused pins on PORTB
	DDRB = PORTB_MASK;    //set all pins on PORTB to output to avoid high-Z
	PORTB |= PORTB_MASK;  //turn on pull-up resistors for unused pins
	
	
	//setup timer 0 for ADC interrupt
	TCCR0B |= 1<<CS00;  //set prescaler to 1/8 cpu clock
	TCCR0B |= 1<<CS01;  
	//TCCR0B |= 1<<CS02; 
	TCCR0A |= 1<<WGM00; //set to PWM Phase correct with a top of OCR0A
	TCCR0A |= 1<<WGM02; 

	OCR0A = 0x0F; //set analog sample frequency
	//TIMSK0 |= 1<<TOIE0; //enable interrupt on counter0 overflow 2^8
	
	//enable output compare match A interrupt on timer0 - enabling this messes with the timer 1 interrupts? on A trying B next...
	ADCSRB |= 1<<ADTS0; // set to interrupt on compare match A timer0A
	ADCSRB |= 1<<ADTS1;
	TIMSK0 |= 1<<OCIE0A; //enable interrupts on output compare for timer 0A
	

	//setup timer1 and counter compare match interrupts
	//configure 16-bit timer for led brightness PWM
	TCCR1B |= 1<<CS10; //prescaling of 1x
	//TCCR1B |= 1<<WGM12; //CTC mode compare match
	TCCR1A |= 1<<WGM10; //set to fast-PWM mode with OCR1A as TOP
	TCCR1A |= 1<<WGM11;
	TCCR1B |= 1<<WGM12;
	TCCR1B |= 1<<WGM13;
	TIMSK1 |= 1<<ICIE1; //enable timer interrupts on timer 1
	TIMSK1 |= 1<<OCIE1A;//enable timer A interrupts on output compare
	///TIMSK1 |= 1<<OCIE1B; //B interrupts on output compare
	//OCR1A = 0xff; //off time
	//OCR1B = 0;

	setupADC();

	//the lower the interrupt register the high the priority for the external interrupt
	PCMSK0 |= 0x48;  //enable pin change interrupts on PCINT4/5 push buttons
	sei();	//enable global interrupts, I-bit
	
	//TODOset interrupt on rising edge for inputs
	
	//set sleep modes need to finish interrupts first/test to wake MCU
	
	clearTimer1();
}

void setDutyCycle2(double onPercentage){
	if(onPercentage >= 0 && onPercentage <= 100){

		maxOnTime = (uint16_t) (((pwmPeriod * onPercentage)*100)/10000);
		maxOffTime = (uint16_t) pwmPeriod - maxOnTime;
	}
}

double convertAnalogToPercentage(uint16_t analogRead){
	return ((double) analogRead * 100.0)/(65536); //reducing steps to convert into percentage
}

uint16_t getKnobAnalogValue(){ //knob that controls the LED PWM input interpolation

	ADCSRA |= 1<<ADSC; //start ADC conversion
	loop_until_bit_is_clear(ADCSRA, ADSC); //ADSC register position will return 0 if there is no conversion in progress
	
	return ADC;//return high byte of left shifted ADC read
}

uint8_t getDigitalInputs(){
	return PINA & PORTA_DIGITAL_SW_MASK; //mask only the digital input pins to read them specifically. will not need once interrupts are finished
}

void rgbwClear(){
	PORTA = PORTA & ~(PORTA_OUTPUT_MASK); //clear all LED related bits
}

void rgbwWrite(bool red, bool green, bool blue, bool white){ 
	//PORTA - PORTA7 PORTA6 PORTA5 PORTA4 PORTA3 PORTA2 PORTA1 PORTA0
	uint8_t writeMask = PORTA &  ~(PORTA_OUTPUT_MASK); // copy PORTA masking out the LEDs
	
	writeMask |= red<<PORTA0;      //write red value into output register
	writeMask |= blue<<PORTA1;     //write blue value into output register
	writeMask |= green <<PORTA2;   //write green value into output register
	writeMask |= white<<PORTA3;    //write white value into output register
	
	PORTA = writeMask; //update PORTA
}

