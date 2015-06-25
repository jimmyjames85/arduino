/* blink.c -- For the Uno */

#include <avr/io.h>
#include <util/delay.h>

enum
{
	BLINK_DELAY_MS = 1000
};

int main(void)
{

	DDRB |= _BV(DDB4); /* set pin 5 of PORTB for output*/

	while (1)
	{
		PORTB |= _BV(PORTB4); /* set pin 5 high to turn led on */
		_delay_ms(BLINK_DELAY_MS);

		PORTB &= ~_BV(PORTB4); /* set pin 5 low to turn led off */
		_delay_ms(BLINK_DELAY_MS);
	}

	return 0; /* CODE SHOULD NEVER REACH HERE */
}
