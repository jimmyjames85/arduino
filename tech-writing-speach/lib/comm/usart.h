/*
 * usart.h
 *
 *  Created on: May 30, 2015
 *      Author: jim
 */

#ifndef INCLUDES_COMM_USART_H_
#define INCLUDES_COMM_USART_H_



#include <stdint.h>

/**
 * Wait indefinitely until a char is received
 */
unsigned char getChar(void);
// unsigned char USART_Receive_with_timeout(unsigned long millis);

/**
 * Wait indefinitely for transfer buffer to be empty, then send char
 */
void sendChar(unsigned char data);
//void USART_Transmit_withDelay(unsigned char data, unsigned long timeoutMillis);

void init_USART(uint32_t baud, uint32_t f_cpu);
//void USART_Init_withMultipleReg(uint32_t baud, uint8_t txRxReg);

void sendString(char * cstr);

#endif /* INCLUDES_COMM_USART_H_ */
