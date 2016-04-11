/*
 * DA3T1.c
 *
 * Created: 4/1/2016 10:10:39
 * Author : Martin
 */ 

// This code waits for a character and transmits the character back (with interrupts)
#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h> // needed for uint8_t
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600
#define ASYNCH_NORM_PRESCALER (F_CPU/16/BAUD - 1)

volatile char ReceivedChar;

void usart0_init (void)
{
	UCSR0B = (1<<TXEN0)  | (1<<RXEN0);	// enable transmit/receive
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	// asynchronous, 8N1
	UBRR0L = ASYNCH_NORM_PRESCALER;		// To set 9600 baud rate with 8MHz clock
}

void usart0_send (unsigned char ch)
{
	while(! (UCSR0A & (1<<UDRE0))); // Polls UCSR0A until UDR0 is ready for write
	UDR0 = ch;
}

void usart0_sendStr(char *data)
{
	unsigned char i;
	for (i = 0; data[i] != NULL; i++)
	{
		usart0_send(data[i]);
	}
}

unsigned char usart0_receive ()
{
	while(!(UCSR0A & (1<<RXC0)));	// poll UCSR0A new data is received.
	return UDR0;					// return received data (stored in UDR0).
}

void adc0_init()
{
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz
	//ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB = 0; // 0 for free running mode
	ADCSRA |= (1 << ADEN); // Enable the ADC
	//ADCSRA |= (1 << ADIE); // Enable Interrupts
	ADCSRA |= (1 << ADSC); // Start the ADC conversion
}

unsigned char adc0_read()
{
	unsigned char adc_data;
	while(ADCSRA & (1<<ADSC));
	adc_data = ADC;
	return adc_data;
}

int main( void )
{
	/*Set baud rate */
	/*
	UBRR0H = (MYUBRR>>8);
	UBRR0L = MYUBRR;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   // Enable receiver and transmitter
	UCSR0B |= (1 << RXCIE0);				 // Enable reciever interrupt
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Set frame: 8data, 1 stp
	sei();
	*/
	
	uint8_t BUFFSIZE = 10;
	char *buffer = "hello world!\n\r"; //[BUFFSIZE];
	uint8_t adc_data;
		
	usart0_init();
	adc0_init();
	
	while(1)
	{
		adc_data = adc0_read();
		//sprintf(buffer, "%u\n", adc_data);
		//usart0_sendStr(buffer);
		usart0_send('H');
		_delay_ms(1000);
	}
}

// ISR (USART_RX_vect)
// {
// 	ReceivedChar = UDR0; // Read data from the RX buffer
// 	UDR0 = ReceivedChar; // Write the data to the TX buffer
// 	//reti();
// }