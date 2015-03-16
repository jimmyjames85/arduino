/*
 * ssd1306.h
 *
 *  Created on: Mar 15, 2015
 *      Author: jim
 */

#ifndef INCLUDES_SSD1306_H_
#define INCLUDES_SSD1306_H_

#include <stdint.h> //for uint8_t


#define COMMAND_BUFFER_SIZE 64
#define ACTUAL_DISPLAY_BUFFER_SIZE 1025
#define DISPLAY_BUFFER_SIZE 1024
#define DISPLAY_ADDR 0x3C
#define SSD_WIDTH 128
#define SSD_HEIGHT 64
#define SSD_PAGE_TOTAL 8


void initScreen();

void setContrast(uint8_t contrast);
void updateDisplay();


void clearScreen();
void setPixel(int x, int y, uint8_t val);
void drawLine(int x1, int y1, int x2, int y2, uint8_t val);

void drawChar(char c, uint8_t col, uint8_t page);

#endif /* INCLUDES_SSD1306_H_ */
