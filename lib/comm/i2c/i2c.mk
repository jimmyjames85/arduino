PROJ_NAME=i2c

SRC_FILE=lib/comm/$(PROJ_NAME)/$(PROJ_NAME).c
OBJ_FILE=obj/$(PROJ_NAME).o

CFLAGS+=-mmcu=$(MCU) -I$(INCLUDES_DIR)

.PHONY: $(PROJ_NAME) 
	
$(PROJ_NAME): folders $(OBJ_FILE)
	
$(OBJ_FILE): $(SRC_FILE)
	$(CC) -Os -DF_CPU=$(CLOCKSPEED) -DMCU=$(MCU) $(CFLAGS) -c $(SRC_FILE) -o obj/$(PROJ_NAME).o
	
#
#i2c_default_target_gaurd:
#	@echo  "\nNo Default Target for I2C_lib available. Only use as a depenency.\n" 
#
#i2c: i2cSrcFile folders obj/i2c.o
#	
#i2cSrcFile: 
#ifeq ($(MCU),atmega328p)
#I2C_SRC_FILE=lib/comm/i2c/i2c.c
#else ifeq ($(MCU), atmega2560)
#I2C_SRC_FILE=lib/comm/i2c/i2c.c
#else
#	$error("No I2C_lib source file available for MCU type: $(MCU) ")
#endif
#
#obj/i2c.o: $(I2C_SRC_FILE)
#	$(CC) -Os -DF_CPU=$(CLOCKSPEED) $(CFLAGS) -c $(I2C_SRC_FILE) -o obj/i2c.o
#