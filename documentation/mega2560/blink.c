/* blink.c -- For the Mega */

#include <avr/io.h>
#include <util/delay.h>

enum { BLINK_DELAY_MS = 1000 };

int main (void)
{

  DDRB |= _BV(DDB7);                /* set pin 7 of PORTB for output*/

  while(1) 
    {
      PORTB |= _BV(PORTB7);        /* set pin 7 high to turn led on */
      _delay_ms(BLINK_DELAY_MS);

      PORTB &= ~_BV(PORTB7);       /* set pin 7 low to turn led off */
      _delay_ms(BLINK_DELAY_MS);
    }


  return 0;                         /* CODE SHOULD NEVER REACH HERE */
}
