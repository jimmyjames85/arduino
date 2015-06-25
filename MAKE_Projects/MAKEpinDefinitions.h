#include "boards.h"

#if MCU == attiny85

#define BOARD_NAME "Attiny85"

#define BUTTON_DDRx DDRB
#define BUTTON_PORTx PORTB
#define BUTTON_PINx PINB
#define BUTTON_bit PB2

#define LED_DDRx DDRB
#define LED_PORTx PORTB
#define LED_PINx PINB
#define LED_bit PB3


#define SPEAKER_DDRx             DDRB
#define SPEAKER_PORTx            PORTB
#define SPEAKER_PINx             PINB
#define SPEAKER_bit                PB0 /* OC0A for CTC mode */


#elif MCU == atmega328p
#define BOARD_NAME "Arduino Uno"

#define BUTTON_DDRx DDRC
#define BUTTON_PORTx PORTC
#define BUTTON_PINx PINC
#define BUTTON_bit PC1      		  /*uno pin A1 */

#define LED_DDRx DDRB
#define LED_PORTx PORTB
#define LED_PINx PINB
#define LED_bit 234     //All should work actually D8 - 13


#define SPEAKER_DDRx             DDRD
#define SPEAKER_PORTx            PORTD
#define SPEAKER_PINx             PIND
#define SPEAKER_bit                PD6        /* uno D6 */  /*TODO set to OC0A for CTC mode */

#elif MCU == atmega2560
#define BOARD_NAME "Arduino MegaADK"

#define BUTTON_DDRx DDRJ
#define BUTTON_PORTx PORTJ
#define BUTTON_PINx PINJ
#define BUTTON_bit PJ0      		  /*mega pin D15*/

#define LED_DDRx DDRB
#define LED_PORTx PORTB
#define LED_PINx PINB
#define LED_bit 234

#else
#define BOARD_NAME ""
#define bad "nonsense"
#define BUTTON_DDRx bad
#define BUTTON_PORTx bad
#define BUTTON_PINx bad
#define BUTTON_bit bad

#define LED_DDRx bad
#define LED_PORTx bad
#define LED_PINx bad
#define LED_bit 234

#define SPEAKER_DDRx             bad
#define SPEAKER_PORTx            bad
#define SPEAKER_PINx             bad
#define SPEAKER_bit                bad

#endif
