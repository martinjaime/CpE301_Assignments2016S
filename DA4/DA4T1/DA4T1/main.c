/*
 * DA4T1.c
 *
 * Created: 4/10/2016 13:11:34
 * Author : Martin
 */ 


#define F_CPU 8000000UL //XTAL = 8 MHz
#include <avr/io.h>
#include <util/delay.h>

#define BOTTOM	0x19	// 8-bit OCR Value for 10% duty cycle
#define TOP		0xE5 	// 8-bit OCR Value for 90% duty cycle
#define OFFSET	0
#define DELAY_MS 5
#define STEP 1			// must be 1 


void fadeRed(unsigned char, unsigned char, char);		// fade output compare register to control red's pwm signal.
void fadeGreen(unsigned char, unsigned char, char);		// fade output compare register to control green's pwm signal.
void fadeBlue(unsigned char, unsigned char, char);		// fade output compare register to control blue's pwm signal.

int main ()
{
	// Set output compare pins as output. 
	DDRD = (1<<DDD5) | (1<<DDD6);
	DDRB = (1<<DDB3);
	// Set up timer0 for colors Red and Green
	// Phase correct, clear on compare match when up counting, and set on compare match when down counting. 
	TCCR0A = (1<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (0<<COM0B0) | (1<<WGM00); // set phase correct mode. 
	TCCR0B = (1<<CS01) | (0<<CS00);			// prescaler of 64
	// Set up timer2 for color Blue
	// Same configuration as timer0
	TCCR2A = (1<<COM2A1) | (0<<COM2A0) | (1<<WGM20);
	TCCR2B = (1<<CS22);						// prescaler of 64
	// Initialize output compare registers to BOTTOM value for 10% duty cycle
	OCR0A = BOTTOM;	// RED
	OCR0B = BOTTOM;	// GREEN
	OCR2A = BOTTOM;	// BLUE
	
	while (1)
	{
		// rgb values traversed as 3 bit grey code
		
		// Increment blue pwm signal	0 0 1
		fadeBlue(BOTTOM, TOP, STEP);
		
		// Increment green pwm signal	0 1 1
		fadeGreen(BOTTOM, TOP, STEP);

		// decrement blue pwm signal	0 1 0
		fadeGreen(TOP, BOTTOM, -STEP);
		
		// Increment red pwm signal		1 1 0
		fadeRed(BOTTOM, TOP, STEP);
		
		// Increment blue pwm signal	1 1 1
		fadeBlue(BOTTOM, TOP, STEP);
		
		// Decrement green pwm signal	1 0 1
		fadeGreen(TOP, BOTTOM, -STEP);
		
		// Decrement blue pwm signal	1 0 0
		fadeBlue(TOP, BOTTOM, -STEP);
		
		// Decrement red pwm signal		0 0 0
		fadeRed(TOP, BOTTOM, -STEP);
		
	}
	return 0;
}

void fadeRed(unsigned char start, unsigned char stop, char step)
{
	for (OCR2A = start; OCR2A != stop; OCR2A += step)
	{
		_delay_ms(DELAY_MS);
	}
}

void fadeGreen(unsigned char start, unsigned char stop, char step)
{
	for (OCR0A = start; OCR0A != stop; OCR0A += step)
	{
		_delay_ms(DELAY_MS);
	}
}

void fadeBlue(unsigned char start, unsigned char stop, char step)
{
	for (OCR0B = start; OCR0B != stop; OCR0B += step)
	{
		_delay_ms(DELAY_MS);
	}
}