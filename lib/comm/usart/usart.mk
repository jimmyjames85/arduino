PROJ_NAME=usart

SRC_FILE=lib/comm/$(PROJ_NAME)/$(PROJ_NAME).c
HEX_FILE=hex/$(PROJ_NAME).hex
BIN_FILE=bin/$(PROJ_NAME).a
OBJ_FILE=obj/$(PROJ_NAME).o

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
	$(CC) -Os $(CFLAGS) -c $(SRC_FILE) -o obj/$(PROJ_NAME).o