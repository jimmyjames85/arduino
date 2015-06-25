/*
 * i2c.h
 * From the book <u>Make: AVR Programming: Learning to write software for hardware</u> by Elliot Williams
 *
 *  Created on: May 31, 2015
 *      Author: jim
 */

#ifndef INCLUDES_COMM_I2C_H_
#define INCLUDES_COMM_I2C_H_
#include <avr/io.h>

/* Sets pullups and initializes bus speed to 100kHz (at FCPU=8MHz) */
void initI2C(void);

/* Waits until the hardware sets the TWINT flag */
void i2cWaitForComplete(void);

/* Sends a start condition (sets (TWSTA) */
void i2cStart(void);

/* Sends a stop condition (sets TWSTO) */
void i2cStop(void);

/* Loads data, sends it out, waiting for completion */
void i2cSend(uint8_t data);

/* Read in from slave, sending ACK when done (sets TWEA) */
uint8_t i2cReadAck(void);

/* Read in from slave, sending NOACK when done (no TWEA) */
uint8_t i2cReadNoAck(void);


#endif /* INCLUDES_COMM_I2C_H_ */
