/*
 * twi.c
 *
 *  Created on: May 30, 2015
 *      Author: jim
 */

#include <avr/io.h>
#include "comm/twi.h"


inline unsigned char getI2CInt()
{
	//sprintf(twi_c_buff, "TWCR = %02x\n", TWCR); //getting 0x34
	//sendString(twi_c_buff);
	return TWCR & (1 << TWINT);
}

inline void assertI2CInt()
{
	TWCR |= (1 << TWINT);
}

void sendI2C(unsigned char i2cAddress, unsigned char * byteArr, int arrSize)
{
	i2cStart();
	i2cSend(i2cAddress<<1);
	int i = 0;
	for (i = 0; i < arrSize; i++)
	{
		i2cSend((uint8_t)byteArr[i]);
	}
	i2cStop();
}

void sendI2COLD(unsigned char i2cAddress, unsigned char * byteArr, int arrSize)
{
	char twi_c_buff[32]={
			'H' , 'E' , 'L' , 'L' , 'O' , ' ' , 'W' , 'O' , 'R' , 'L' , 'D' , ' ' , ':' , '-' , ')' , '\n',
			'H' , 'E' , 'L' , 'L' , 'O' , ' ' , 'W' , 'O' , 'R' , 'L' , 'D' , ' ' , ':' , '-' , ')' , '\0'};

	/**
	 *  See page 226		http://www.atmel.com/images/doc8161.pdf  Uno
	 *  See page 251		http://www.atmel.com/images/doc2549.pdf  Mega_Adk
	 *
	 */
	//TWBR = 0x1F;		//make it fast i guess
	//TWSR &= (0xFC);		//clear prescaler bits
						//TODO this is for TWI what is it? and why do we need it?




//sendChar('1');
	/* STEP #1  ------------------------------------------------------------*/
	//        (enable)      (START )      (Interupt)
	TWCR |= (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);

//sendChar('2');
	/* STEP #2  ------------------------------------------------------------*/
	while (!getI2CInt())
		;

//sendChar('3');
	/* STEP #3  ------------------------------------------------------------*/
	//if((TWSR & 0xF8) != START)
		//sendString("ERROR Step3");// <----- TODO CHECK STATUS

	//unsigned char c = (TWSR & 0xF8);
	//sprintf(twi_c_buff,"\n(TWSR & 0xF8) = %02x \n",c) ;
	//sprintf(twi_c_buff,"i2cAddress = %02x \n",i2cAddress) ;
	//sprintf(twi_c_buff, "Hello?\n");
	//sendString(twi_c_buff);

	//Load i2c i2cAddress and R/W bit  (SLA+R/W)
	TWDR = (i2cAddress << 1);	//Write

	//Enable and SEND!!!
	TWCR |= (1 << TWINT) | (1 << TWEN);

//sendChar('4');
	/* STEP #4    wait for START condition ---------------------------------*/
	while (!getI2CInt())
		;
//sendChar('5');
	/* STEP #5  Load Data  -------------------------------------------------*/

	//if((TWSR & 0xF8) != MT_SLA_ACK) ERROR(); <----- TODO CHECK STATUS (might need todo this in the for loop bellow as well)

	int i = 0;
	for (i = 0; i < arrSize; i++)
	{
		TWDR = byteArr[i];                      //Load byte from array into TWDR REgister
		TWCR = (1 << TWINT) | (1 << TWEN); 		//Clear TWINT to start transmission of data

		/* STEP #6 Wait for ACK  ------------------------------------------*/
		while (!getI2CInt())
			;
	}
//sendChar('7');
	/* STEP #7  Load Data  -------------------------------------------------*/
	//if((TWSR & 0xF8) != MT_DATA_ACK) ERROR(); <----- TODO CHECK STATUS

	//     (Interupt)      (enable)     (STOP)
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	   //Transmit Stop
//sendChar('\n');
//sendChar('\n');
}
