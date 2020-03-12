#Main application file name
TARGET = Counter
#Main hex file path in windows format
BUILD_DIR = build

MAIN_HEX_PATH = $(BUILD_DIR)/$(TARGET).hex

MCU = atmega16

# Compiler and other Section

GCC_PATH = /mnt/c/toolchain/avr-gcc-9.2.0-x64-mingw/bin
# GCC_PATH = C:/toolchain/avr-gcc-9.2.0-x64-mingw/bin
PREFIX = avr-

CC = $(GCC_PATH)/$(PREFIX)gcc.exe
OBJCOPY = $(GCC_PATH)/$(PREFIX)objcopy.exe
OBJDUMP = $(GCC_PATH)/$(PREFIX)objdump.exe
SZ = $(GCC_PATH)/$(PREFIX)size.exe
AVRDUDE := $(GCC_PATH)/avrdude.exe

#Options for avr-gcc
CFLAGS = -O3 -Werror -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall

#Linking options for avr-gcc
LFLAGS = -O3 -mmcu=$(MCU) -Wl,--gc-sections

#Options for HEX file generation
HFLAGS = -j .text -j .data -O ihex

#Options for avrdude to burn the hex file
#MMCU model here according to avrdude options
DUDEFLAGS = -c avr109
DUDEFLAGS += -p m32u4  
DUDEFLAGS += -P COM4
DUDEFLAGS += -b 1200
DUDEFLAGS += -U flash:w:$(MAIN_HEX_PATH):i

# Sources files needed for building the application 
SRC = main.c
SRC += code/lib/semseg.c

# The headers files needed for building the application
INCLUDE = -I. 
INCLUDE += ./code/lib/


all : $(BUILD_DIR)/$(TARGET).elf	
	$(OBJCOPY) $(HFLAGS) $< $(BUILD_DIR)/$(TARGET).hex
	$(OBJDUMP) -h -S $< > $(BUILD_DIR)/$(TARGET).lss
	
$(BUILD_DIR)/$(TARGET).elf: $(BUILD_DIR)/main.o
	@echo $<
	$(CC) $< $(INCLUDE) $(LFLAGS) -o $@
	$(SZ) $@
	 
$(BUILD_DIR)/main.o: code/main.c Makefile | $(BUILD_DIR)
	@echo $<
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR):
	mkdir $@
# main:
# 	@echo $(CC)


#######################################
# clean up
#######################################
.PHONY: clean
clean:
	-rm -fR $(BUILD_DIR)
  
# commands Section
.PHONY: flash
flash : 	
	$(AVRDUDE) $(DUDEFLAGS)

