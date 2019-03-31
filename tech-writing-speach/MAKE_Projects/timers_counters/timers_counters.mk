timers_counters_z_defaultTargetGaurd:
	@echo "No default targets available."

include lib/comm/usart/usart.mk 

PROJ_NAME=timers_counters
SRC_FILE=MAKE_Projects/$(PROJ_NAME)/$(PROJ_NAME).c

PUTTY_PROFILE=arduino_$(BOARD)
UPLOAD_MAIN=$(UPLOADER) -v -p $(PARTNO) -c $(PROGRAMMER_ID) -P $(UPLOADER_PORT) -U flash:w:hex/$(PROJ_NAME).hex
UPLOAD_MAIN_GAURDED=if  ps -e | grep $(UPLOADER) > /dev/null ; then echo "ABORT: $(UPLOADER) is running" ;  exit ; else $(UPLOAD_MAIN); fi;
RUN_PUTTY=putty -load $(PUTTY_PROFILE)&

timers_counters: hex/$(PROJ_NAME).hex
	
timers_counters_upload_start_putty: timers_counters_upload
	$(RUN_PUTTY)

timers_counters_upload: hex/$(PROJ_NAME).hex
	$(UPLOAD_MAIN_GAURDED)

hex/timers_counters.hex: folders bin/$(PROJ_NAME).a
	$(OBJCP) -O ihex -R .eeprom bin/$(PROJ_NAME).a hex/$(PROJ_NAME).hex

bin/timers_counters.a: obj/$(PROJ_NAME).o obj/usart.o
	$(CC) $(CFLAGS) obj/$(PROJ_NAME).o obj/usart.o -o bin/$(PROJ_NAME).a

obj/timers_counters.o: $(SRC_FILE)
	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c $(SRC_FILE) -o obj/$(PROJ_NAME).o
