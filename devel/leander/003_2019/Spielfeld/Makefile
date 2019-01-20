PORT ?= /dev/ttyACM0
TARGET=main

LIB=.


DEVICE = atmega328p
MCU = atmega328p
AVRDUDE_DEVICE = atmega328p

#OBJ=$(patsubst %.c,%.o,$(wildcard *.c))
#OBJ+=$(TARGET).o
OBJ+=$(patsubst %.c,%.o,$(wildcard $(LIB)/*.c))

OPT=-O3
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) $(DEVICE_SPECIFIC_CFLAGS) $(OPT) -std=c99 -fdata-sections -ffunction-sections
CC=avr-gcc
LDFLAGS=-Wl,-gc-sections -Wl,-relax

#-lpololu_$(DEVICE) 

AVRDUDE=avrdude

#.PRECIOUS: %.elf

all:$(TARGET).hex memory-used
	
clean:
	rm -f *.o *.hex *.obj *.hex *.elf *.i *.s *.eep

%.eep: %.elf
	 avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex $< $@

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@ 

%.elf: $(OBJ)
	avr-gcc $(CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

%.obj: %.o
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

#%.o: %.c
	#$(CC) $(CFLAGS) $< -o $@

program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c stk500v2 -P $(PORT) -U flash:w:$(TARGET).hex -U eeprom:w:$(TARGET).eep -B 40
	
memory-used:$(TARGET).elf
	avr-size --mcu=$(MCU) -C $(TARGET).elf
#	avr-nm -S	$(OBJ)
