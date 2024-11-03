# MCU and clock speed
MCU=atmega8
F_CPU=8000000UL

# Compiler and linker flags
CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

# Source and output files
SRC=SED1200EMU.c lcd.c
OBJ=$(SRC:.c=.o)
TARGET=SED1200EMU

# AVRDUDE programming options
PROGRAMMER=usbasp  # Adjust based on your programmer
PORT=/dev/ttyUSB0  # Adjust if needed
AVRDUDE_FLAGS=-c $(PROGRAMMER) -p $(MCU) -P $(PORT)

# Build process
all: $(TARGET).hex

%.o: %.c
	avr-gcc $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	avr-gcc $(CFLAGS) $(OBJ) -o $@

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

# Clean up intermediate files
clean:
	rm -f *.o *.elf *.hex

# Flash to ATmega
flash: $(TARGET).hex
	avrdude $(AVRDUDE_FLAGS) -U flash:w:$(TARGET).hex:i
