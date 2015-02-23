/* ATMEGA 2560 */
/*-mmcu=avr6               https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html*/
/*-mmcu=atmega2560		   https://balau82.wordpress.com/2011/03/29/programming-arduino-uno-in-pure-c/  */
/*-DF_CPU=16000000UL*/
/* http://johanneshoff.com/arduino-command-line.html  */
/*
 avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o led.o led.c
 avr-gcc -mmcu=atmega328p led.o -o led
 avr-objcopy -O ihex -R .eeprom led led.hex
 avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:led.hex
 */
/* avrdude ==>> http://www.nongnu.org/avrdude/user-manual/avrdude_4.html */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "sugar.h"

//#define F_CPU 16000000

//UBRRn  = USART Baud Rate Register
//UCSRnA = USART Control and Status Register A

//0=input
//1=output
void setPinDirection(unsigned pin, uint8_t direction)
{
	if (pin == 13)
	{
		if (direction)
			DDRB |= (unsigned char) (0xA0); /*set pin 13 to output*/
		else
			DDRB &= (0X1F);/*set pin 13 to input*/
	}
}

void writePin(unsigned pin, uint8_t value)
{
	if (pin == 13)
	{
		if (value)
			PORTB |= (unsigned char) 0xA0; /* set pin 13 high */
		else
			PORTB &= (unsigned char) 0x1F; /* set pin 13 high */

	}


}

void initSerial(uint8_t txRxReg, uint32_t baud)
{
	unsigned long UBRRnCalc = ((F_CPU / 16 / baud) - 1); //UBRRn = USART Baud Rate Register

	if (txRxReg == 0)
	{
		UBRR0H = (unsigned char) UBRRnCalc >> 8;
		UBRR0L = (unsigned char) UBRRnCalc;

		UCSR0B = (1 << RXEN0) | (1 << TXEN0);
		/* Set frame format: 8data, 2stop bit */
		UCSR0C = (1 << USBS0) | (3 << UCSZ00);
	}

	//UCSR0B = (1 << TXEN0) | (1 << TXCIE0);
	//UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void sendChar(unsigned char data)
{
	/* wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)))
		;

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void sendStr(char * str)
{
	unsigned len = strlen(str);
	int i;
	for (i = 0; i < len; i++)
	{
		sendChar(str[i]);
	}
	delay(20);
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	//int timeout = 9999;
//	while (!(UCSR0A & (1 << RXC0)) );//&& timeout != 0)
	//timeout--;
	if (!(UCSR0A & (1 << RXC0)))
		return '\0';

	/* Get and return received data from buffer */
	return UDR0;
}

#define BUFFER_SIZE 1024
unsigned char bufferIn[BUFFER_SIZE + 1];

unsigned bip = 0;
unsigned bop = 0;

int main(void)
{
	delay(50)
	bufferIn[BUFFER_SIZE] = '\0';

	initSerial(0, 57600);
	setPinDirection(13, 1);
	writePin(13, 0);

	unsigned bip = 0;
	unsigned bop = 0;
	sendStr("howdy partner \r\n");

	char str[BUFFER_SIZE];
	sprintf(str, "address of PORTB is %d\r\n", &PORTB);
	sendStr(str);


	int * addr = &PORTB;
	int bit = 7;

	while (1)
	{
		*addr |= (unsigned char) 0xA0;
		delay(500);
		*addr &= (unsigned char) 0x1F;
		delay(500);
	};

//	PORTB |= (unsigned char) 0xA0; /* set pin 13 high */
//	PORTB &= (unsigned char) 0x1F; /* set pin 13 low*/

	while (1)
	{
		while (UCSR0A & (1 << RXC0))
		{
			unsigned char c = USART_Receive();

			bufferIn[bip++] = c;
			if (c == '\r' || c == '\n' || bip == BUFFER_SIZE)
			{
				bufferIn[bip] = '\0';
				sendStr("\r\n");
				sendStr(bufferIn);
				sendStr("\r\n");
				bip = 0;

			}
			writePin(13, 1);
		}

		writePin(13, 0);
	}

	return 0;
}
