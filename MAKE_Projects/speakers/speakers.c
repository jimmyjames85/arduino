/*
 * speaker.c
 *
 *  Created on: Jun 11, 2015
 *      Author: Jim's version of MAKE: AVR Programing
 *
 *
 *
 * (Assumes clock is at 1MHZ)
 */

#include "../MAKEpinDefinitions.h"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <stdint.h>
#include <stdio.h>
#include "comm/usart.h"

#include "notes.h"
#include "8bitNotes.h"
#define MARY_HAD_A_LITTLE_LAMB { E6, D6, C6, D6, E6, E6, E6, E6, D6, D6, D6, D6, E6, G6, G6, G6, E6, D6, C6, D6, E6, E6, E6, E6, D6, D6, E6, D6, C6, C6, C6 , C6 }

const uint16_t song[] = MARY_HAD_A_LITTLE_LAMB;

#define SONG_TOTAL_NOTES sizeof(song)/sizeof(uint16_t)

void playNote(uint16_t wavelength, uint16_t duration)
{

	uint16_t elapsed;
	uint16_t i;
	for (elapsed = 0; elapsed < duration; elapsed += wavelength)
	{
		for (i = 0; i < wavelength; i++)
		{
			_delay_us(1);
		}
		SPEAKER_PORTx ^= (1 << SPEAKER_bit);
	}
}

void ledOn(void)
{
	LED_PORTx |= _BV(LED_bit);
}
void ledOff()
{
	LED_PORTx &= ~_BV(LED_bit);
}

void setupSpeaker(void)
{
	SPEAKER_DDRx |= _BV(SPEAKER_bit); /* Turn speaker on */
}
void setupLed(void)
{
	LED_DDRx |= _BV(LED_bit); //setup led port
}

void oldSpeakersCh5(void)
{
	setupSpeaker();
	int i = 0;
	while (1)
	{
		for (i = 0; i < SONG_TOTAL_NOTES; i++)
		{
			playNote(song[i], 1000);
		}

		_delay_ms(500);
	}
}


static inline void initTimer(void)
{
	TCCR0A |= _BV(WGM01); /* CTC Mode = Clear Timer on Compare when timer TC0 equals OCCR0A */

	TCCR0A |= _BV(COM0A0); /* Actually we can clear on compare or toggle on compare
	 	 	 	 	 	     this sets it to toggle which is what we want since we
	 	 	 	 	 	     we are moving the speaker cone both back and forth. */

	TCCR0B |= _BV(CS00) | _BV(CS01); /* CPU clock / 64   */

}


void playNoteWithTimer(uint8_t wavelength, uint16_t duration)
{
	OCR0A = wavelength; /* we only have 8 bits of resolution */
	SPEAKER_DDRx |= _BV(SPEAKER_bit); //enable speaker output

	while(duration)
	{
		_delay_ms(1); /* <---- Note difference from old playNote's _delay_us(1) */
		duration--;
	}
	SPEAKER_DDRx &= ~_BV(SPEAKER_bit);  /*Turn speaker off*/

}


int main(void)
{
//	setupSpeaker();
//	oldSpeakersCh5();
	initTimer();
	while(1)
	{

		playNoteWithTimer(C3_8, 200);

		playNoteWithTimer(E3_8, 200);

		playNoteWithTimer(G3_8, 200);

		playNoteWithTimer(C3_8, 400);


		_delay_ms(1000);
		_delay_ms(1000);
		_delay_ms(1000);
	}



	//clock_prescale_set(clock_div_8); //arduino is too fast for us :-) this sets clock speed to 2MHz


}

