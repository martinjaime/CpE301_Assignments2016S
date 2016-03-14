/*
 * DA2T3_C.c
 *
 * Created: 3/13/2016 18:31:02
 * Author : Martin
 */ 

#define F_CPU 8000000UL //XTAL = 8 MHz
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char x = 1<<DDC0;		// waveform var
	unsigned char counter = 0;		// counter to output
	// I decided to use pinC 4 and 5 instead since pin 6 is used for reset.
	DDRC = (1<<DDC4) | (1<<DDC5);
	DDRB = 0xFF;					// set PORTB as output
	PORTB = counter;				// output the count to PORTB
	while (1)
	{
		_delay_ms(250);			// delay for 0.25 s
		x ^= 1<<PORTC0;			// toggle x
		if (x == 1) {			// if x is on rising edge,
			PORTB = ++counter;	// increment count.
			if (counter % 5 == 0) {	// if counter is div by 5,
				PORTC ^= (1<<PINC5);	// Toggle pinC5
				if (counter % 10 == 0)	// if also div by 10, 
					PORTC ^= (1<<PINC4);	// toggle pinC4
			}
		} // end if(x == 1)
	} // end while
}
