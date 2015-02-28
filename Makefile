
MCU			= atmega328p
CPU_FREQ	= 16000000UL
AVRDUDE_MCU	= m328p
PROGRAMMER	= dragon_isp

PROJECT		= nanoTracer

OBJS		= \
	drivers/adc.o \
	drivers/serial.o \
	main.o	\
	tracer.o
	
%.o: %.c
	@echo 'Building file: $<'
	avr-gcc -Iinclude -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -DF_CPU=$(CPU_FREQ) -c -o "$@" "$<"
	@echo ' '

all: $(PROJECT).elf

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
	avrdude -p $(AVRDUDE_MCU) -c $(PROGRAMMER) -P usb -U flash:w:$(PROJECT).hex:i

# Other Targets
clean:
	-$(RM) *.o *.d drivers/*.o drivers/*.d $(PROJECT).map $(PROJECT).elf $(PROJECT).hex
	-@echo ' '