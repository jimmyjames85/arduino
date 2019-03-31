
#define atmega328p 1
#define atmega2560 2


#if BOARD_TYPE == atmega328p
#define BOARD_NAME "Arduino Uno"

#define F_CPU 2000000UL  //Override!!!!!  ARDUINO's run at 16MHz and we are using clock_div_8
#define BUTTON_DDRx DDRC
#define BUTTON_PORTx PORTC
#define BUTTON_PINx PINC
#define BUTTON_bit PC1      		  /*uno pin A1 */

#define LED_DDRx DDRB
#define LED_PORTx PORTB
#define LED_PINx PINB
#define LED_bit 234     //All should work actually D8 - 13


#define SPEAKER_DDRx             DDRD
#define SPEAKER_PORTx            PORTD
#define SPEAKER_PINx             PIND
#define SPEAKER_bit                PD6                            /* uno D6 */

#elif BOARD_TYPE == atmega2560
#define BOARD_NAME "Arduino MegaADK"
#define F_CPU 2000000UL  //Override!!!!!  ARDUINO's run at 16MHz and we are using clock_div_8
#define BUTTON_DDRx DDRJ
#define BUTTON_PORTx PORTJ
#define BUTTON_PINx PINJ
#define BUTTON_bit PJ0      		  /*mega pin D15*/

#define LED_DDRx DDRB
#define LED_PORTx PORTB
#define LED_PINx PINB
#define LED_bit 234

#else
#define BOARD_NAME ""
#define bad "nonsense"
#define BUTTON_DDRx bad
#define BUTTON_PORTx bad
#define BUTTON_PINx bad
#define BUTTON_bit bad

#define LED_DDRx bad
#define LED_PORTx bad
#define LED_PINx bad
#define LED_bit 234 //not using this D18 through D21 and D38

#endif


#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <stdint.h>
#include <stdio.h>
#include "comm/usart.h"


void printMilliseconds(uint16_t value)
{
	/* Given a value in milliseconds, prints out how many seconds
	 you took over the serial port.  Does ascii conversion, prints
	 decimal point, and drops extra leading zeros.
	 */
	uint8_t digit;

	sendString("\nYou took ");
	/* add up ten-thousands */
	digit = 0;
	while (value >= 10000)
	{
		digit++;
		value -= 10000;
	}
	if (digit)
	{
		sendChar('0' + digit);
	}
	/* add up thousands */
	digit = 0;
	while (value >= 1000)
	{
		digit++;
		value -= 1000;
	}
	sendChar('0' + digit);

	/* decimal point here b/c dividing by 1000 */
	sendChar('.');

	/* add up hundreds */
	digit = 0;
	while (value >= 100)
	{
		digit++;
		value -= 100;
	}
	sendChar('0' + digit);
	/* add up tens */
	digit = 0;
	while (value >= 10)
	{
		digit++;
		value -= 10;
	}
	sendChar('0' + digit);

	// Ones digit is easy.
	sendChar('0' + value);

	sendString(" seconds.\n");
}

void printComments(uint16_t value)
{
	/* Given a value in milliseconds, rates your reaction time */
	if (value > 2000)
	{
		sendString("---->  Ummm...this is a reaction timer...\n");
	}
	else if (value > 1000)
	{
		sendString("---->  Hello?\n");
	}
	else if (value > 500)
	{
		sendString("---->  Slow.\n");
	}
	else if (value > 250)
	{
		sendString("---->  Have another cup of coffee.\n");
	}
	else if (value > 200)
	{
		sendString("---->  Respectable.\n");
	}
	else if (value >= 150)
	{
		sendString("---->  Fast.\n");
	}
	else if (value < 150)
	{
		sendString("---->  Amazing!\n");
	}
}

void randomDelay(void)
{
	/* Waits for a "random" delay from 1 - 3.5 sec */
	/* Requires timer 1 initialized and running */
	/* It's not really random, but very hard to control --
	 like coin-flipping. */

	uint8_t randomTime;

	_delay_ms(1000); /* wait at least 1 sec */
	randomTime = (uint8_t) TCNT1;
	/* type-casting the 16-bit TCNT1 as an 8-bit number keeps
	 only the 8 least-significant (fastest-changing) bits  */
	while (--randomTime)
	{
		_delay_ms(10); /* max value is 255 ~= 2.5 sec */
	}

}



static inline void initTimer1(void)
{
	/* Normal mode (default), just counting */
	TCCR1B |= (1 << CS12) | (1 << CS10); 	/*Set Clock Speed to: F_CPU / 1024 */
	/* No special output modes */
}





int main(void)
{
	clock_prescale_set(clock_div_8); // arduino is too fast for us :-) this sets clock speed to 2MHz
									 // which means we must override/define F_CPU above

	uint32_t timerValue;

	// -------- Inits --------- //
	initTimer1();
	init_USART_override(9600, F_CPU); //<--- See this is overridden above

	LED_DDRx = 0xff; /* all LEDs for output */
	BUTTON_PORTx |= (1 << BUTTON_bit); /* enable internal pull-up */



	sendString("\nReaction Timer:\n");
	sendString("---------------\n");
	sendString("Press any key to start.\n");

	char millisString[50] = {0};
	// ------ Event loop ------ //
	while (1)
	{
		getChar(); /* press any key */
		sendString("\nGet ready...");
		randomDelay();
		sendString("\nGo!\n");
		LED_PORTx = 0xff; /* light LEDs */

		TCNT1 = 0; /* reset counter */

		if (bit_is_clear(BUTTON_PINx, BUTTON_bit))
		{
			/* Button pressed _exactly_ as LEDs light up.  Suspicious. */
			sendString("You're only cheating yourself.\n");
		}
		else
		{
			// Wait until button pressed, save timer value.
			loop_until_bit_is_clear(BUTTON_PINx, BUTTON_bit);
			timerValue= TCNT1;

			timerValue *= 512;
			timerValue /= 1000;

			printMilliseconds(timerValue);
			printComments(timerValue);
		}


		LED_PORTx = 0x00; // Clear LEDs and start again.
		sendString("Press any key to try again.\n");

	} /* End event loop */
	return (0); /* This line is never reached */
}
