#PROGRAMMER = avrisp2
#PROGRAMMER = usbtiny
PROGRAMMER=usbtiny
CPU_GCC = atmega8
CPU_DUDE = m8

F_CPU = 16000000


CDEFS = -DF_CPU=$(F_CPU)
CFLAGS = -mmcu=$(CPU_GCC) $(CDEFS) -Wall -Os

PROGRAM = firmware

.phony: clean

all: $(PROGRAM).hex

program: $(PROGRAM).hex
	avrdude -c $(PROGRAMMER) -P usb -p $(CPU_DUDE) -U flash:w:$(PROGRAM).hex

clean:
	rm *.o *.elf *.hex

$(PROGRAM).hex: $(PROGRAM).c
	avr-gcc $(CFLAGS) -c $(PROGRAM).c  -o $(PROGRAM).o
	avr-gcc $(CFLAGS) $(PROGRAM).o -o $(PROGRAM).elf
	avr-objcopy -R .eeprom -O ihex $(PROGRAM).elf $(PROGRAM).hex

fuse:
	avrdude -c $(PROGRAMMER) -P usb -p $(CPU_DUDE) -U lfuse:w:0x3f:m -U hfuse:w:0xd9:m 
