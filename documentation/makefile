CC=avr-gcc
OBJCP=avr-objcopy

MCU=atmega328p
PARTNO=m328p

CFLAGS=-mmcu=$(MCU) 
UPLOAD=avrdude -F -v -p m2560 -c stk500 -P /dev/ttyACM0 -U flash:w:hex/blink.hex
UPLOADPUTTY=avrdude -F -v -p m2560 -c stk500 -P /dev/ttyACM0 -U flash:w:hex/blink.hex; putty -load arduino

all: blink.hex


uploadPutty: 
	if  ps -e | grep "avrdude" > /dev/null ; then echo "ABORT: avrdude is running" ;  exit ; else if ps -e | grep "putty" > /dev/null ; then echo "ABORT: putty is running" ;  exit ; else $(UPLOADPUTTY); fi ; fi;
	
upload:
	if  ps -e | grep "avrdude" > /dev/null ; then echo "ABORT: avrdude is running" ;  exit ; else $(UPLOAD); fi; 


	
blink.hex: folders blink.a
	$(OBJCP) -O ihex -R .eeprom bin/blink.a hex/blink.hex

blink.a: blink.o ssd1306.o
	$(CC) $(CFLAGS) obj/ssd1306.o obj/blink.o -o bin/blink.a

blink.o:  src/blink.c 
	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c src/blink.c -o obj/blink.o

ssd1306.o: src/ssd1306.c	
	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c src/ssd1306.c -o obj/ssd1306.o	 

folders:
	if ! [ -d "./obj" ]; then mkdir obj ; fi; if ! [ -d "./bin" ]; then mkdir bin ; fi; if ! [ -d "./hex" ]; then mkdir hex ; fi  
	
clean:
	rm -rf obj bin hex
