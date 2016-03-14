/*
 * DA2T1_c.c
 *
 * Created: 3/12/2016 21:14:34
 * Author : Martin
 */ 

#define F_CPU 8000000UL //XTAL = 8 MHz
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 1<<DDC0;		// Set PC0 as an output
	PORTC = 1<<PORTC0;	// Set PC0 high
    while (1) 
    {
		_delay_ms(250);		// delay for 0.25 s
		PORTC ^= 1<<PORTC0;	// toggle PC0
    }
}

