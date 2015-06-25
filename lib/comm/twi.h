/*
 * twi.h
 *
 *  Created on: May 30, 2015
 *      Author: jim
 */

#ifndef INCLUDES_COMM_TWI_H_
#define INCLUDES_COMM_TWI_H_

#include "i2c.h"
void sendI2C(unsigned char i2cAddress, unsigned char * byteArr, int arrSize);

#endif /* INCLUDES_COMM_TWI_H_ */
