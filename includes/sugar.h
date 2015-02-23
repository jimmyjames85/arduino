/*
 * sugar.h
 *
 *  Created on: Feb 22, 2015
 *      Author: jim
 */

#ifndef INCLUDES_SUGAR_H_
#define INCLUDES_SUGAR_H_

#ifdef __HAS_DELAY_CYCLES
#define delay(milli) _delay_ms(milli);
#endif

enum PIN_MODE{OUTPUT, INPUT};

//void pinMode(unsigned int pin, enum PIN_MODE);

#endif /* INCLUDES_SUGAR_H_ */

