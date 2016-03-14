/*
 * DA2T2_C.c
 *
 * Created: 3/12/2016 23:17:23
 * Author : Martin
 */ 

#define F_CPU 8000000UL //XTAL = 8 MHz
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned char x = 1<<DDC0;		// waveform var
	unsigned char counter = 0;		// counter to output
	DDRB = 0xFF;					// set PORTB as output
	PORTB = counter;				// output the count to PORTB
	while (1)
	{
		_delay_ms(250);		// delay for 0.25 s
		x ^= 1<<PORTC0;		// toggle x
		if (x == 1)			// if x is on rising edge, 
			PORTB = ++counter;	// increment count.
	}
}
