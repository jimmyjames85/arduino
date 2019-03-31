/*
 * i2c.c
 *
 * From the book <u>Make: AVR Programming: Learning to write software for hardware</u> by Elliot Williams
 *
 *  Created on: May 31, 2015
 *      Author: jim
 */

#include "boards.h"
#include <avr/io.h>
//#define MCU attiny85

#if MCU == attiny85
#define SCL  2   //<-------- this must be clocked in software :-0
#define SDA  5
#endif


/* Sets pullups and initializes bus speed to 100kHz (at FCPU=8MHz) */
void initI2C(void)
{
#if MCU == attiny85

	//setup 2 Wire Mode
	USIWM = 2;
	DDRB &= _BV(SCL);
	DDRB &= _BV(SDA);

#else
	TWBR = 32; 				/* set bit rate, see p. 242 */
			   				/* 8MHz / (16 + 2*TWBR*1) ~= 100kHz */

	TWCR |= (1 << TWEN); 	/* enable */
#endif
}

/* Waits until the hardware sets the TWINT flag */
void i2cWaitForComplete(void)
{
	loop_until_bit_is_set(TWCR, TWINT);
}

/* Sends a start condition (sets (TWSTA) */
void i2cStart(void)
{

#if MCU == attiny85

	PORTB &= ~_BV(SDA); //hold SDA low
	PORTB |= _BV(SCL); //while SCL is held high
	//USIDR &= ~_BV(7); //alternative



	//PORTB
	//DDRB |= _BV(PB3);
	//DDRB |= _BV(7);

#else
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTA));

	i2cWaitForComplete();
#endif
}

/* Sends a stop condition (sets TWST0) */
void i2cStop(void)
{
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO));
}

/* Loads data, sends it out, waiting for completion */
void i2cSend(uint8_t data)
{
	TWDR = data;
	TWCR = (_BV(TWINT) | _BV(TWEN)); /* init and enable */
	i2cWaitForComplete();
}

/* Read in from slave, sending ACK when done (sets TWEA) */
uint8_t i2cReadAck(void)
{
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWEA));
	i2cWaitForComplete();
	return (TWDR);
}

/* Read in from slave, sending NOACK when done (no TWEA) */
uint8_t i2cReadNoAck(void)
{
	TWCR = (_BV(TWINT) | _BV(TWEN));
	i2cWaitForComplete();
	return (TWDR);
}
