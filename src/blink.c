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

enum
{
	BLINK_DELAY_MS = 5000,
};

int main(void)
{

	/*PB7 = pin 13 ==onboard led*/

	DDRB = 0xFFFF; /*set pin 13 to output*/
	PORTB = 0xFFFF; /* set pin 13 high */


	while (1)
	{

		PORTB = 0xFFFF; /* set pin 13 high */
		_delay_ms(BLINK_DELAY_MS);

		PORTB = 0x0000; /* set pin 13 low */
		_delay_ms(BLINK_DELAY_MS);
	}

	return 0;
}
