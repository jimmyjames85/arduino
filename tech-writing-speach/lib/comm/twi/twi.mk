PROJ_NAME=twi

SRC_FILE=lib/comm/$(PROJ_NAME)/$(PROJ_NAME).c
HEX_FILE=hex/$(PROJ_NAME).hex
BIN_FILE=bin/$(PROJ_NAME).a
OBJ_FILE=obj/$(PROJ_NAME).o

ifdef UPLOADER_PORT
UPLOAD_MAIN=$(UPLOADER) -v -p $(PARTNO) -c $(PROGRAMMER_ID) -P $(UPLOADER_PORT) -U flash:w:hex/$(PROJ_NAME).hex
else
UPLOAD_MAIN=$(UPLOADER) -v -p $(PARTNO) -c $(PROGRAMMER_ID) -U flash:w:hex/$(PROJ_NAME).hex
endif

UPLOAD_MAIN_GAURDED=if  ps -e | grep $(UPLOADER) > /dev/null ; then echo "ABORT: $(UPLOADER) is running" ;  exit ; else $(UPLOAD_MAIN); fi;


CFLAGS+=-mmcu=$(MCU) -I$(INCLUDES_DIR)

.PHONY: $(PROJ_NAME) upload_$(PROJ_NAME) 

upload_$(PROJ_NAME): folders $(HEX_FILE)
	$(UPLOAD_MAIN_GAURDED)
	
$(PROJ_NAME): folders $(HEX_FILE)

$(HEX_FILE): $(BIN_FILE)
	$(OBJCP) -O ihex -R .eeprom bin/$(PROJ_NAME).a hex/$(PROJ_NAME).hex 

$(BIN_FILE): $(OBJ_FILE)
	$(CC) $(CFLAGS) obj/$(PROJ_NAME).o -o bin/$(PROJ_NAME).a
	
$(OBJ_FILE): $(SRC_FILE)
	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c $(SRC_FILE) -o obj/$(PROJ_NAME).o
	

#twi_default_target_gaurd:
#	@echo  "\nNo Default Target for TWI_lib available. Only use as a depenency.\n" 
#
#twi: twiSrcFile folders obj/twi.o
#	
#twiSrcFile: 
#ifeq ($(MCU),atmega328p)
#TWI_SRC_FILE=lib/comm/twi/twi.c
#else ifeq ($(MCU), atmega2560)
#TWI_SRC_FILE=lib/comm/twi/twi.c
#else
#	$error("No TWI_lib source file available for MCU type: $(MCU) ")
#endif
#	
#obj/twi.o:  $(TWI_SRC_FILE)
#	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c $(TWI_SRC_FILE) -o obj/twi.o

