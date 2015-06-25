#I think when you include this makefile from another it will automatically run the first target

pickTarget:
	@echo  "\nPlease select make target:\n\t\t\t\t blink\n\t\t\t\t uploadBlink\n\n\t\t\t\t (MCU is $(MCU))" 

blink: blinkSrcFile folders hex/blink.hex
	
blinkSrcFile: 
ifeq ($(MCU),atmega328p)
BLINK_SRC_FILE=blink/blink_uno.c
else ifeq ($(MCU), atmega2560)
BLINK_SRC_FILE=blink/blink_megaAdk.c
else ifeq ($(MCU), attiny85)
BLINK_SRC_FILE=blink/blink_attiny85.c
else
	$error("No src file available for MCU type: $(MCU) ")
endif
	

uploadBlink: blink
	$(UPLOADER) -v -p $(PARTNO) -c $(PROGRAMMER_ID) -P $(UPLOADER_PORT) -U flash:w:hex/blink.hex 
	
hex/blink.hex: bin/blink.a
	$(OBJCP) -O ihex -R .eeprom bin/blink.a hex/blink.hex

bin/blink.a: obj/blink.o
	$(CC) $(CFLAGS) obj/blink.o -o bin/blink.a

obj/blink.o:  $(BLINK_SRC_FILE)
	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c $(BLINK_SRC_FILE) -o obj/blink.o
#	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c blink/blink.c -o obj/blink.o
	
