/*
 * lab3prog1.c
 *
 * Created: 14/02/2012 11:53:59 AM
 * Author: Ethan
 */ 

#include <avr/io.h>

#define F_CPU 16000000

#include <avr/delay.h>
#include <math.h>

// val is from ADC
int remap(int val, int min_val, int max_val)
{
	int result = ((val-min_val)/(max_val-min_val))*255;

	if(result<0)		//If ADC value is below min value
		return 0;
	
	if(result>255)		//If ADC value is above max value
		return 255;
	
	return result;
}

void tone(int input)
{
	//i rep the number of wave within 1 sec
	int i, freq; 
	double period;
	
	freq = (input/255)*400+100;   //100 is the least freq, 500 is the highest freq
	period = 1/freq;			  //eg. 0.01s
	
	for(i=0; i< freq; i++)
	{
		// Write a 1 to digital pin 13 (PINB 5)
		PINB|= 0b00100000;
		_delay_ms(period/2*pow(10,3));
		//Delay is half the period (in millisecond)
	
		// Write a 0 to digital pin 13 (PINB 5)
		PINB &= 0b11011111;
		_delay_ms(period/2*pow(10,3));
		//Delay is half the period (in millisecond)
	}
}

int main(void)
{
	unsigned int adcval, loval, hival, constrained;

	//Set GPIO Pins
	DDRB|=0b00100000;  //PIN 13 output to buzzer
	
	//PRR to ADC (0 to ON)
	PRR &= 0b11111110;
	
	//ON ADC (1 to ON)
	ADCSRA |= 0b10000000;
	
	//Set PS Value to Std Sampling Freq 125khz
	ADCSRA |= 0b00000111;
	
	//Set Ref Voltage (Vcc) and Channel Mux (Chn 1)
	ADMUX |= 0b01000001;
	
	while(1)
	{
		//Start Conversion
		ADCSRA |= 0b01000000;
		
		//Poll till complete conversion
		while(ADCSRA & 0b01000000);
		
		//Clear the Interrupt Flag
		ADCSRA |= 0b00010000;
		
        loval=ADCL;
		hival=ADCH;
		adcval= hival * 256 + loval;
		
		//0-255 steps for tone strength 
	    constrained  = remap(750,620,890);
		
		//Output tone )
		tone(00);
	}	
}