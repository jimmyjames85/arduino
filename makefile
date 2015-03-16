MCU=atmega2560
CLOCKSPEED=16000000UL
#m2560

#http://www.nongnu.org/avrdude/user-manual/avrdude_4.html
#-p
#avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:led.hex
 
#avrdude -F -v -p m2560 -c arduino -P /dev/ttyACM0 -U flash:w:hex/blink.hex


CC=avr-gcc
OBJCP=avr-objcopy
CFLAGS=-mmcu=$(MCU) -Iincludes
AVRDUDE_RUNNING=`ps -e | grep "eclipse" > /dev/null`
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
