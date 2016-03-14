/*
 * DA2T4_C.c
 *
 * Created: 3/13/2016 22:58:52
 * Author : Martin
 */ 

#define F_CPU 8000000UL //XTAL = 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char counter = 0;		// counter to output

ISR(PCINT1_vect) 
{
	if (PINC0 == 1) {
		PORTB = ++counter;
		if (counter % 5 == 0) {	// if counter is div by 5,
			PORTC ^= (1<<PINC5);	// Toggle pinC5
			if (counter % 10 == 0)	// if also div by 10,
				PORTC ^= (1<<PINC4);	// toggle pinC4
		}	
	}
}

int main(void)
{
	//unsigned char x = 1<<DDC0;		// waveform var
	// I decided to use pinC 4 and 5 instead since pin 6 is used for reset.
	DDRC = (1<<DDC0) | (1<<DDC4) | (1<<DDC5);
	DDRB = 0xFF;					// set PORTB as output
	PORTB = counter;				// output the count to PORTB
	PCMSK1 |= (1<<PCINT8);
	PCICR  |= (1<<PCIE1);
	sei();
	
	while (1)
	{
		_delay_ms(250);			// delay for 0.25 s
		PORTC ^= 1<<PORTC0;			// toggle x
		//if (x == 1) {			// if x is on rising edge,
			//PORTB = ++counter;	// increment count.
			/*if (counter % 5 == 0) {	// if counter is div by 5,
				PORTC ^= (1<<PINC5);	// Toggle pinC5
				if (counter % 10 == 0)	// if also div by 10,
				PORTC ^= (1<<PINC4);	// toggle pinC4
			}*/
		//} // end if(x == 1)
	} // end while
}

