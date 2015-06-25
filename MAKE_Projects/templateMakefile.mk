PROJ_NAME=template

SRC_FILE=MAKE_Projects/$(PROJ_NAME)/$(PROJ_NAME).c
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
	