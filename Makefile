# Compile flags
F_CPU ?= 16000000
MCU ?= atmega328p

# ISP programmer flags
ISP_MCU = $(MCU)
ISP_PROGRAMMER ?= arduino
AVRDUDE_FLAGS = -q -q -p $(ISP_MCU) -c $(ISP_PROGRAMMER)
ifdef ISP_PORT
	AVRDUDE_FLAGS += -P $(ISP_PORT)
endif

# Make flags
CC = avr-gcc
LDFLAGS = -Wl,-Map,firmware.map
COMPILEFLAGS = -Wall -Werror -g -Os -mmcu=$(MCU) \
	-DF_CPU=$(F_CPU)
CFLAGS = -std=c99 $(COMPILEFLAGS)
ASFLAGS = $(COMPILEFLAGS)

SOURCES = main.c display.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all
all: firmware.hex

.PHONY: clean
clean:
	rm -f firmware.elf firmware.hex $(OBJECTS)

.PHONY: burn
burn: firmware.hex
	avrdude $(AVRDUDE_FLAGS) -U $<

firmware.hex: firmware.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

firmware.elf: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)
