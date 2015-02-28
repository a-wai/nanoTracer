
PROGRAMMER	= arduino
PROG_PORT	= /dev/ttyACM0
MCU			= atmega328p
CPU_FREQ	= 16000000UL

PROJECT		= nanoTracer

OBJS		= \
	src/drivers/adc.o \
	src/drivers/serial.o \
	src/main.o	\
	src/tracer.o
	
%.o: %.c
	@echo 'Building file: $<'
	avr-gcc -Iinclude -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -DF_CPU=$(CPU_FREQ) -c -o "$@" "$<"
	@echo ' '

all: $(PROJECT).hex

# Tool invocations
$(PROJECT).elf: $(OBJS)
	@echo 'Building target: $@'
	avr-gcc -mmcu=$(MCU) -o $@ $(OBJS) $(USER_OBJS) $(LIBS)
	@echo ' '

$(PROJECT).hex: $(PROJECT).elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex $(PROJECT).elf  $(PROJECT).hex
	@echo ' '

size: $(PROJECT).elf
	-avr-size --format=avr --mcu=$(MCU) $(PROJECT).elf

upload: $(PROJECT).hex
	@echo 'Writing Flash image to device'
	avrdude -p $(MCU) -c $(PROGRAMMER) -P $(PROG_PORT) -U flash:w:$(PROJECT).hex:i

# Other Targets
clean:
	-$(RM) src/*.o src/drivers/*.o $(PROJECT).elf $(PROJECT).hex
	-@echo ' '
