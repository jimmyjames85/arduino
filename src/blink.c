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
#include <compat/twi.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "sugar.h"
#include <avr/io.h>
#include <avr/iom2560.h>
#include <avr/iomxx0_1.h>

#include "ATmega2560_ArduinoPinMap.h"
/*
 #define SSD1306_LCDWIDTH      128
 #define SSD1306_LCDHEIGHT      64
 #define SSD1306_SETCONTRAST   0x81
 #define SSD1306_DISPLAYALLON_RESUME 0xA4
 #define SSD1306_DISPLAYALLON 0xA5
 #define SSD1306_NORMALDISPLAY 0xA6
 #define SSD1306_INVERTDISPLAY 0xA7
 #define SSD1306_DISPLAYOFF 0xAE
 #define SSD1306_DISPLAYON 0xAF
 #define SSD1306_SETDISPLAYOFFSET 0xD3
 #define SSD1306_SETCOMPINS 0xDA
 #define SSD1306_SETVCOMDETECT 0xDB
 #define SSD1306_SETDISPLAYCLOCKDIV 0xD5
 #define SSD1306_SETPRECHARGE 0xD9
 #define SSD1306_SETMULTIPLEX 0xA8
 #define SSD1306_SETLOWCOLUMN 0x00
 #define SSD1306_SETHIGHCOLUMN 0x10
 #define SSD1306_SETSTARTLINE 0x40
 #define SSD1306_MEMORYMODE 0x20
 #define SSD1306_COLUMNADDR 0x21
 #define SSD1306_PAGEADDR   0x22
 #define SSD1306_COMSCANINC 0xC0
 #define SSD1306_COMSCANDEC 0xC8
 #define SSD1306_SEGREMAP 0xA0
 #define SSD1306_CHARGEPUMP 0x8D
 #define SSD1306_EXTERNALVCC 0x1
 #define SSD1306_SWITCHCAPVCC 0x2 */
//#define F_CPU 16000000
//UBRRn  = USART Baud Rate Register
//UCSRnA = USART Control and Status Register A
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
	_delay_ms(20);
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

void setPinDirection(volatile uint8_t * ddrAddr, int pinMask, int direction)
{
	//0=input
	//1=output

	if (direction)	//set output
		*ddrAddr |= (1 << pinMask);
	else
		*ddrAddr &= ~(1 << pinMask);
}

void writePin(volatile uint8_t * portAddr, int pinMask, int value)
{
	if (value)	//high
		*portAddr |= (1 << pinMask);
	else
		*portAddr &= ~(1 << pinMask);
}
void error(char * msg)
{
	sendStr(msg);
	while (1)
		;

}

inline unsigned char getI2CInt()
{
	return TWCR & (1 << TWINT);
}
inline void assertI2CInt()
{
	TWCR |= (1 << TWINT);
}
char str[BUFFER_SIZE];

void displayI2Cstatus()
{
	return;
	sprintf(str, " TWBR: %02x\r\n", TWBR);
	sendStr(str);
	sprintf(str, " TWSR: %02x\r\n", TWSR);
	sendStr(str);
	sprintf(str, " TWEN: %02x\r\n", (TWCR & (1 << TWEN)));
	sprintf(str, "TWINT: %02x\r\n-------\r\n", getI2CInt());
	sendStr(str);

}

void sendI2C(unsigned char i2cAddress, unsigned char * byteArr, int arrSize)
{
	//STEP #1
	//      (enable)    (START )     (Interupt)
	TWCR |= (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);

	//Step #2
	while (!getI2CInt())
		;

	//Step #3
	//TODO CHECK STATUS (TWSR & 0xF8)=?
	//Load i2c i2cAddress and R/W bit  (SLA+R/W)
	TWDR = (i2cAddress << 1);	//Write
	//Enable and SEND!!!
	TWCR |= (1 << TWINT) | (1 << TWEN);

	//Step #4 wait...
	while (!getI2CInt())
		;

	//Step #5 Load Data
	//TODO CHECK STATUS (TWSR & 0xF8)=?
	int i = 0;
	for (i = 0; i < arrSize; i++)
	{
		TWDR = byteArr[i];
		TWCR = (1 << TWINT) | (1 << TWEN);
		//Step #6 wait for ACK
		while (!getI2CInt())
			;
	}

	//Transmit Stop
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

#define COMMAND_BUFFER_SIZE 64
#define BUFFER_SIZE 1024

static uint8_t cmdBuffer[COMMAND_BUFFER_SIZE + BUFFER_SIZE] =
{ 0 };
static uint8_t * buffer = &cmdBuffer[COMMAND_BUFFER_SIZE];

void setContrast(uint8_t contrast)
{
	cmdBuffer[0] = 0x00;	//continous commands only
	cmdBuffer[1] = 0x81;	//contrast cmd
	cmdBuffer[2] = contrast;	//value
	sendI2C(0x3C, cmdBuffer, 3);
}

void updateDisplay()
{

	memset(cmdBuffer, 0xe3, COMMAND_BUFFER_SIZE);	//fill with no ops

	cmdBuffer[0] = 0x00;	//continous command
	cmdBuffer[1] = 0x20;	//set mode
	cmdBuffer[2] = 0x00;	//to horizontal

	cmdBuffer[3] = 0x21;	//set column address
	cmdBuffer[4] = 0x00;	//to use the entire screen
	cmdBuffer[5] = 0xFF;	//to use the entire screen

	cmdBuffer[6] = 0x22;	//set page address
	cmdBuffer[7] = 0x00;	//to use the entire screen
	cmdBuffer[8] = 0xFF;	//to use the entire screen

	sendI2C(0x3C, &cmdBuffer[0], 16);

	cmdBuffer[15] = 0x40;	//continous ram data
	sendI2C(0x3C, &cmdBuffer[15], 1025);

}

void clearScreen()
{
	memset(buffer, 0, BUFFER_SIZE);	//clear buffer
}

void setPixel(int x, int y)
{
	int width = 127;
	int height = 63;

	if (x < 0 || y < 0 || x > width || y > height )
		return;


}

void drawLine(int x1, int y1, int x2, int y2)
{
	int width = 127;
	int height = 63;

	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x1 > width || y1 > height || x2 > width || y2 > height)
		return;

	if(x1>x2)
	{
		int tempX=x1;
		int tempY=y1;
		x1=x2;
		y1=y2;
		x2=tempX;
		y2=tempY;
	}


	double rise = y2 - y1;
	double run = x2 - x1;

	double nextX = x1;
	double nexyY = y1;

	while(nextX<=x2)
	{

	}
}



int main(void)
{

initSerial(0, 57600);

TWBR = 0x00;		//make it fast i guess
TWSR &= (0xFC);		//clear prescaler bits

_delay_ms(500);

clearScreen();
updateDisplay();

//not sure about this
//TWDR = 0xC0;	//0xC0 := databytes and commands requires
//TWDR = 0x40;	//0x40 := stream of databytes
//setContrast(0xFF);
int i = 0;
int j = 0;
int y = 0;
for (j = 0; j < 8; j++)
	for (i = 0; i < 127; i++)
	{
		unsigned char bar[] =
		{ 0x40, (1 << (y++ % 8)) };
		sendI2C(0x3C, bar, 2);
	}

setContrast(0x00);
_delay_ms(500);
setContrast(0xFF);
_delay_ms(500);
setContrast(0x00);
_delay_ms(500);
setContrast(0xFF);
_delay_ms(500);

clearScreen();
updateDisplay();

setPinDirection(&DP12DDR, DP12MASK, 1);

bufferIn[BUFFER_SIZE] = '\0';
unsigned bip = 0;
while (1)
{
	while (UCSR0A & (1 << RXC0))
	{
		unsigned char ch = USART_Receive();

		bufferIn[bip++] = ch;
		if (ch == '\r' || ch == '\n' || bip == BUFFER_SIZE)
		{
			bufferIn[bip] = '\0';
			sendStr("\r\n");
//				sendStr(bufferIn);
			//			sendStr("\r\n");

			unsigned char command[BUFFER_SIZE];
			int cmdc = 0;
			int ci = 0;
			//unsigned c=0;
			while (ci < bip)
			{
				int cmd;

				char chr = bufferIn[ci++];
				if (chr >= '0' && chr <= '9')
					cmd = ((chr - '0') << 4);
				else if (chr >= 'a' && chr <= 'f')
					cmd = ((10 + chr - 'a') << 4);
				else if (chr >= 'A' && chr <= 'F')
					cmd = ((10 + chr - 'A') << 4);
				else
					continue;

				//sendChar(ch);
				chr = bufferIn[ci++];

				if (chr >= '0' && chr <= '9')
					cmd |= ((chr - '0'));
				else if (chr >= 'a' && chr <= 'f')
					cmd |= ((10 + chr - 'a'));
				else if (chr >= 'A' && chr <= 'F')
					cmd |= ((10 + chr - 'A'));
				else
					continue;

				//sendChar(ch);

				//sprintf(str, "  cmd=%02x \r\n", cmd);
				//sendStr(str);
				command[cmdc++] = cmd;
			}

			bip = 0;

			for (ci = 0; ci < cmdc; ci++)
			{
				sprintf(str, "%02x ", command[ci]);
				sendStr(str);

			}
			sendStr("\r\n");
			sendI2C(0x3C, command, cmdc);

			bip = 0;
		}
	}
}

while (1)
{
	DP13PORT |= (1 << DP13MASK);
	writePin(&DP12PORT, DP12MASK, 1);
	_delay_ms(500);
	DP13PORT &= ~(1 << DP13MASK);
	writePin(&DP12PORT, DP12MASK, 0);
	_delay_ms(500);

};
return 0;
}
