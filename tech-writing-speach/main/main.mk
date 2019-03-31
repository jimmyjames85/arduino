main_defaultTargetGaurd:
	@echo "No default targets available."

include lib/comm/usart/usart.mk 

PUTTY_PROFILE=arduino_$(BOARD)
UPLOAD_MAIN=$(UPLOADER) -v -p $(PARTNO) -c $(PROGRAMMER_ID) -P $(UPLOADER_PORT) -U flash:w:hex/main.hex
UPLOAD_MAIN_GAURDED=if  ps -e | grep $(UPLOADER) > /dev/null ; then echo "ABORT: $(UPLOADER) is running" ;  exit ; else $(UPLOAD_MAIN); fi;
RUN_PUTTY=putty -load $(PUTTY_PROFILE)&

main: hex/main.hex
	
uploadMain_StartPutty: uploadMain
	$(RUN_PUTTY)

uploadMain: hex/main.hex
	$(UPLOAD_MAIN_GAURDED)

hex/main.hex: folders bin/main.a
	$(OBJCP) -O ihex -R .eeprom bin/main.a hex/main.hex

bin/main.a: obj/main.o obj/usart.o
	$(CC) $(CFLAGS) obj/main.o obj/usart.o -o bin/main.a

obj/main.o: main/main.c
	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c main/main.c -o obj/main.o
