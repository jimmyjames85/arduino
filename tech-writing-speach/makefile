#!!!! If you change the board you must run: make clean
MCU=attiny85
#attiny85
#atmega328p
#attiny85
#uno
#megaAdk

PROGRAMMER_ID=usbasp
#usbasp
#arduino

include ./boardSpecifics/$(MCU)_$(PROGRAMMER_ID).mk

UPLOADER=avrdude
CC=avr-gcc
OBJCP=avr-objcopy
MAKE=make
INCLUDES_DIR=lib

.PHONY: folders clean

folders: #create obj bin and hex folders
	@if ! [ -d "./obj" ]; then mkdir obj ; fi; if ! [ -d "./bin" ]; then mkdir bin ; fi; if ! [ -d "./hex" ]; then mkdir hex ; fi  
	
clean:
	rm -rf obj bin hex;

include MAKE_Projects/speakers/*.mk
