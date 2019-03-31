/*
 * usart.c
 *
 *  Created on: May 30, 2015
 *      Author: jim
 */

#include <avr/io.h>
#include "comm/usart.h"

unsigned char getChar(void)
{
	/**
	 *  USART_Receive(void)
	 *  See page 187 - 188		http://www.atmel.com/images/doc8161.pdf  Uno
	 *  See page 215 - 216		http://www.atmel.com/images/doc2549.pdf  Mega_Adk
	 */

	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)) );

	/* Get and return received data from buffer */
	return UDR0;
}

void sendChar(unsigned char data)
{
	/**
	 *  USART_Transmit(unsigned char data)
	 *  See page 184      http://www.atmel.com/Images/doc8161.pdf  Uno
	 *  See page 212	  http://www.atmel.com/images/doc2549.pdf  Mega_Adk
	 */

	/* wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)));

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void init_USART(uint32_t baud, uint32_t f_cpu)
{
	/**
	 *  See page 183    http://www.atmel.com/Images/doc8161.pdf  Uno
	 *  See page 211    http://www.atmel.com/images/doc2549.pdf  Mega_Adk
	 */

	/**
	 * 	UBRRn is the USART Baud Rate Register
	 *  Some boards have multiple USARTS (e.g. atmega2560)
	 *  The n in UBRRn points to the USART you are referring to (e.g. UBBR0 or UBBR1)
	 *  This method only uses the 0th USART
	 */

	//See page 208 for calculation
	unsigned long UBRRn_BaudRateCalculation = ((f_cpu / 16 / baud) - 1); //for all n in [0,1,2,3]

	/* Set baud rate */
	UBRR0H = (unsigned char) UBRRn_BaudRateCalculation >> 8;
	UBRR0L = (unsigned char) UBRRn_BaudRateCalculation;


	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);

}

void sendString(char * cstr)
{
	unsigned long i=0;
	for(i=0;cstr[i]!='\0';i++)
		sendChar(cstr[i]);

/*	while(*cstr != '\0')
	{
		sendChar(*cstr);
		cstr++;
	}*/

}

