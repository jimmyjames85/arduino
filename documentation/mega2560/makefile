CC=avr-gcc
OBJCP=avr-objcopy


CLOCKSPEED=16000000UL
MCU=atmega328p 
PARTNO=m328p
PROGRAMMER_ID=arduino #stk500v2

PORT=/dev/ttyUSB0

CFLAGS=-Os -DF_CPU=$(CLOCKSPEED) -mmcu=$(MCU) 

#UPLOAD=avrdude -F -v -p m2560 -c stk500 -P /dev/ttyACM0 -U flash:w:hex/blink.hex
UPLOAD=avrdude -p $(PARTNO) -c $(PROGRAMMER_ID) -P $(PORT) -U flash:w:blink.hex

UPLOADPUTTY=avrdude -F -v -p m2560 -c stk500 -P /dev/ttyACM0 -U flash:w:hex/blink.hex; putty -load arduino

#all: blink.hex

all: step5

step5: step4
	avrdude -v -p m2560 -c stk500v2 -P /dev/ttyACM0 -U flash:w:blink.hex
step4: step3
	avr-objcopy -O ihex -R .eeprom blink.a blink.hex
step3: step2
	avr-gcc -mmcu=atmega2560 blink.o -o blink.a
step2: step1
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega2560 -c blink.c -o blink.o
step1: blink.c
	touch blink.c




upload: blink.hex
	if  ps -e | grep "avrdude" > /dev/null ; then echo "ABORT: avrdude is running" ;  exit ; else $(UPLOAD); fi; 

blink.hex: blink.a
	$(OBJCP) -O ihex -R .eeprom blink.a blink.hex

blink.a: blink.o
	$(CC) $(CFLAGS) blink.o -o blink.a

blink.o: blink.c 
	$(CC) $(CFLAGS) -c blink.c -o blink.o

clean:
	rm blink.o blink.a blink.hex
