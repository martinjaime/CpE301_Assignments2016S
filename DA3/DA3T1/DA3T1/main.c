/*
 * DA3T1.c
 *
 * Created: 4/1/2016 10:10:39
 * Author : Martin
 */ 

#define F_CPU		8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE	9600
#define ASYNCH_NORM_PRESCALER (F_CPU/16/BAUDRATE - 1)
#define MSGSIZE		39

void usart_init (void)
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);				// enable transmission.
	UCSR0C = ((1<<UCSZ01) | (1<<UCSZ00) /*| (1<<UMSEL00)*/);	// asynchronous, 8N1
	UBRR0H = (ASYNCH_NORM_PRESCALER>>8);		// To set 9600 baud rate with 8MHz clock
	UBRR0L = ASYNCH_NORM_PRESCALER;

}

void usart_send (unsigned char ch)
{
	while(!(UCSR0A & (1 << UDRE0))); // Polls UCSR0A until UDR0 is ready for write
	UDR0 = ch;
}

int main(void)
{
	unsigned char str[MSGSIZE] = "CpE301: Microcontroller System Design\n\r";
	unsigned char strLenght = MSGSIZE;
	unsigned char i = 0;
	usart_init();
	while(1)
	{
		usart_send(str[i++]); // iterate through string and pass char to function
		if (i>=strLenght)
		{
			i = 0;
			_delay_ms(500);
		}
	}
	return 0;
}