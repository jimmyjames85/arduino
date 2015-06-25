#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <stdint.h>
#include <stdio.h>

#include "comm/usart.h"
#define atmega328p 1
#define atmega2560 2

#if BOARD_TYPE == atmega328p
#define BOARD_NAME "Arduino Uno"
#define capSensorCallback ISR(PCINT1_vect)
//#define CAP_SENSOR_INTvect  PCINT1_vect
#define CAP_SENSOR_PCIEx  PCIE1
#define CAP_SENSOR_PIN  PC1      		  /*uno pin A1 */
#define CAP_SENSOR_PCINTx PCINT9
#define CAP_SENSOR_DDR  DDRC
#define CAP_SENSOR_PORT PORTC
#define CAP_SENSOR_PCMSKx PCMSK1
#define CAP_SENSOR_PCIFx PCIF1

#elif BOARD_TYPE == atmega2560
#define BOARD_NAME "Arduino MegaADK"
#define capSensorCallback ISR(PCINT1_vect)
//#define CAP_SENSOR_INTvect  PCINT1_vect
#define CAP_SENSOR_PCIEx  PCIE1
#define CAP_SENSOR_PIN  PJ0      		  /*mega pin D15 */
#define CAP_SENSOR_PCINTx PCINT9
#define CAP_SENSOR_DDR  DDRJ
#define CAP_SENSOR_PORT PORTJ
#define CAP_SENSOR_PCMSKx PCMSK1
#define CAP_SENSOR_PCIFx PCIF1

#else

#define BOARD_NAME ""
#define capSensorCallback void fakeISR(PCINT1_vect)
#define CAP_SENSOR_PCIEx  bad
#define CAP_SENSOR_PIN  dont
#define CAP_SENSOR_PCINTx use
#define CAP_SENSOR_DDR  this
#define CAP_SENSOR_PORT is
#define CAP_SENSOR_PCMSKx only
#define CAP_SENSOR_PCIFx bad

#endif


#define LED_DDR  DDRD
#define LED_PORT PORTD


#define SENSE_TIME 50
#define THRESHOLD 7000


volatile uint16_t chargeCycleCount;



/*
 *
+---------------------+-----------------------+--------------------------+
|  Datasheet Name     |  Vector name for ISRs |  Which pins are covered  |
+---------------------+-----------------------+--------------------------+
|PCINT0               |PCINT0_vect            |PB0...PB7                 |
+---------------------+-----------------------+--------------------------+
|PCINT1               |PCINT1_vect            |PC0...PC7                 |
+---------------------+-----------------------+--------------------------+
|PCINT2               |PCINT2_vect            |PD0...PD7                 |
+---------------------+-----------------------+--------------------------+

*/


void initPinChangeInterrupt(void)
{
	/*
	 PCIE0  pin-change interrupt for B pins
	 PCIE1  pin-change interrupt for C pins
	 PCIE2  pin-change interrupt for D pins

	 PCMSK0  B pin mask
	 PCMSK1  C pin mask
	 PCMSK2  D pin mask

	 PCICR stays the same
	 */


	PCICR |= (1 << CAP_SENSOR_PCIEx);             /* set enable pin change interupt enable */
	CAP_SENSOR_PCMSKx |= (1 << CAP_SENSOR_PCINTx);     /* set mask for our specific pin */
}

//this has to be set by hand!!!!!!!!! non inserted variables

//ISR(PCINT1_vect)
capSensorCallback
{
	//CAP_SENSOR_INTvect)
	chargeCycleCount++;

	CAP_SENSOR_DDR |= (1 << CAP_SENSOR_PIN); /*output mode*/
	_delay_us(1);	   					 /*charge delay*/

	CAP_SENSOR_DDR &= ~(1 << CAP_SENSOR_PIN); /*set as input*/
	PCIFR |= (1 << CAP_SENSOR_PCIFx);				  /*clear the pin-change interrupt */

}




int main(void)
{
	clock_prescale_set(clock_div_1); /* full speed */
	init_USART(9600);


	sendString("==[ Cap Sensor ]==\n\n");


	LED_DDR = 0xff;      /* set for output */
	MCUCR |= (1 << PUD); /* disable all pullups */

	CAP_SENSOR_PORT |= (1 << CAP_SENSOR_PIN );          /* we can leave output high */

	initPinChangeInterrupt();


	char outputString[50] = {0};

	while(1)
	{
		chargeCycleCount = 0; 					/* reset counter */
		CAP_SENSOR_DDR |= (1 << CAP_SENSOR_PIN);  	/* start with cap charged */
		sei();									/* set enable interrupts */
		_delay_ms(SENSE_TIME);
		cli();									/* clear set enable interrupts */
		if(chargeCycleCount < THRESHOLD)
		{
			LED_PORT = 0xff;
		}
		else
		{
			LED_PORT = 0;
		}

		sprintf(outputString, "%s chargeCycleCount %d\n", BOARD_NAME, chargeCycleCount);
		sendString(outputString);
	}

	return 0;	/*Code should never reach here */




}
