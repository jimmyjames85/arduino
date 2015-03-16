/*
 * ssd1306.c
 *
 *  Created on: Mar 15, 2015
 *      Author: jim
 */
#include <string.h> //for memset
#include <util/delay.h>
#include "ssd1306.h"
#include "twi.h"
#include "ledFont.h"

static uint8_t cmdBuffer[COMMAND_BUFFER_SIZE] =
{ 0 };
static uint8_t actualDisplayBuffer[DISPLAY_BUFFER_SIZE + 1] =
{ 0 };
static uint8_t * displayBuffer = &actualDisplayBuffer[1];

void setContrast(uint8_t contrast)
{
	int tot = 0;
	cmdBuffer[tot++] = 0x00;	//continous commands only
	cmdBuffer[tot++] = 0x81;	//contrast cmd
	cmdBuffer[tot++] = contrast;	//value
	sendI2C(DISPLAY_ADDR, cmdBuffer, tot);
}

void updateDisplay()
{
	int tot = 0;
	cmdBuffer[tot++] = 0x00;	//continous command
	cmdBuffer[tot++] = 0x20;	//set mode
	cmdBuffer[tot++] = 0x00;	//to horizontal

	//START AT 0,0
	cmdBuffer[tot++] = 0x21;	//set column address
	cmdBuffer[tot++] = 0x00;	//to use the entire screen
	cmdBuffer[tot++] = 0xFF;	//to use the entire screen

	//START AT 0,0
	cmdBuffer[tot++] = 0x22;	//set page address
	cmdBuffer[tot++] = 0x00;	//to use the entire screen
	cmdBuffer[tot++] = 0xFF;	//to use the entire screen

	sendI2C(DISPLAY_ADDR, cmdBuffer, tot);

	actualDisplayBuffer[0] = 0x40;	//continous ram data
	sendI2C(DISPLAY_ADDR, actualDisplayBuffer, ACTUAL_DISPLAY_BUFFER_SIZE);
}

void clearScreen()
{
	memset((void *) actualDisplayBuffer, 0, ACTUAL_DISPLAY_BUFFER_SIZE);	//clear buffer
	actualDisplayBuffer[0] = 0x40;	//continous ram data
}

void setPixel(int x, int y, uint8_t val)
{

	if (x < 0 || y < 0 || x >= SSD_WIDTH || y >= SSD_HEIGHT)
		return;

	int page = y / SSD_PAGE_TOTAL;
	int arrIndex = SSD_WIDTH * page + x;

	uint8_t mask = (1 << y % 8);
	if (val)
		displayBuffer[arrIndex] |= mask;
	else
		displayBuffer[arrIndex] &= ~mask;
}

void drawLine(int x1, int y1, int x2, int y2, uint8_t val)
{
	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x1 >= SSD_WIDTH || y1 >= SSD_HEIGHT || x2 >= SSD_WIDTH || y2 >= SSD_HEIGHT)
		return;

	if (x1 == x2)
	{
		//vertical line
		if (y1 < y2)
			while (y1 <= y2)
				setPixel(x1, y1++, val);
		else
			while (y2 <= y1)
				setPixel(x1, y2++, val);

		return;
	}
	else if (x1 > x2)
	{
		int tempX = x1;
		int tempY = y1;
		x1 = x2;
		y1 = y2;
		x2 = tempX;
		y2 = tempY;
	}

	double rise = y2 - y1;
	double run = x2 - x1;

	int nextX = x1;
	int nextY = y1;

	while (nextX <= x2)
	{
		setPixel(nextX, nextY, val);
		nextX++;
		nextY = ((nextX * rise / run)) + y1;	//y1 is the y intercept
	}
}

void drawChar(char c, uint8_t col, uint8_t page)
{
	int j = 0;
	for (j = 0; j < 5; j++)
	{
		displayBuffer[page * SSD_WIDTH + col + j] = font[5 * (int) c + j];
	}
}

void initScreen()
{
	//http://elegantcircuits.com/2014/08/15/ssd1306-to-atmega328p-using-i2c-protocol/
	//{0xAE,0x00 | 0x0,0x10 | 0x0,0x40 | 0x0,0x81,0xCF,0xA1,0xC8,0xA6,0xA4,0xA8,0x3F,0xD3,0x0,0xD5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,0x40,0x20,0x00,0x40 | 0x0,0x8D,0x14,0xA4,0xAF}
	unsigned char arr[] =
	{ 0x00, 0xAE, 0x00 | 0x0, 0x10 | 0x0, 0x40 | 0x0, 0x81, 0xCF, 0xA1, 0xC8, 0xA6, 0xA4, 0xA8, 0x3F, 0xD3, 0x0, 0xD5, 0x80, 0xD9, 0xF1, 0xDA, 0x12, 0xDB, 0x40, 0x20, 0x00, 0x40 | 0x0, 0x8D, 0x14, 0xA4, 0xAF };

	sendI2C(DISPLAY_ADDR, arr, 30);
}
