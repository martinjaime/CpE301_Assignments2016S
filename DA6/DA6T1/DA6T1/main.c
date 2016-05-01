/*
 * DA6T1.c
 *
 * Created: 4/30/2016 16:31:22
 * Author : Martin
 */ 

#define F_CPU 8000000UL		// Set clock frequency

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "nokia5110.h"		// include LCD API

void ADC_init();			// Function to initialize ADC

int main(void)
{
	nokia_lcd_init();		// Initialize LCD
	ADC_init();				// Initialize ADC 

	sei();					// enable interrupts. 

	while(1);				// Infinite loop waiting for interrupts. 
}

void ADC_init() 
{
	DDRC &= ~(0<<DDC0);		// Clear bit 0 of DDRC
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz
	ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB = 0; // 0 for free running mode
	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADIE); // Enable Interrupts
	ADCSRA |= (1 << ADSC); // Start the ADC conversion
}

ISR(ADC_vect)
{
	uint16_t ADC_value = ADC;	// read ADC conversion
	uint32_t temperature_C, temperature_F;
	char *header = "Temp(F) LM35:";
	char buffer[10];
	
	// Convert ADC value to temperature in Celcius
	temperature_C = (int32_t)(ADC_value * 5 * 100 / 1024.0);
	// Convert Celcius to Farenheit 
	temperature_F = temperature_C * 1.8 + 32;
	if(temperature_F < 120)
	// There is noise at output that jumps to a very large value. 
	// This condition is to avoid that. 
	{
		sprintf(buffer, "%u", temperature_F); // write value to buffer as string
		nokia_lcd_clear();	// clear LCD
		nokia_lcd_set_cursor(0, 0); // reset cursor
		nokia_lcd_write_string(header,1); // write message header
		nokia_lcd_set_cursor(0, 10); // move cursor down. 
		nokia_lcd_write_string(buffer, 3); // Write buffer. 
		nokia_lcd_write_string(" o", 1); // print letter 'o' as degrees symbol. 
		nokia_lcd_render();	// render the message to LCD. 
	}
	_delay_ms(1000); // delay for 1 second. 
}