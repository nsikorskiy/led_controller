#Контроллер, установленный на плате. Может быть другим, например atmega328 
DEVICE     = atmega328p

#Тактовая частота 16 МГц 
CLOCK      = 16000000

#Команда запуска avrdude. Ее нужно скопировать из Arduino IDE.
#AVRDUDE = /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avrdude -C/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf -carduino -P/dev/tty.usbserial-A600dAAQ -b19200 -D -p atmega168
AVRDUDE = /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -v -v -v -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D

MINICOM = minicom -o   -D /dev/ttyACM0

OBJECTS    = led_controller.o twi.o uart485.o api.o ds3231.o

#COMPILE = avr-gcc -gdwarf-2 -gpubnames -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all:led_controller.hex

.c.o:
		$(COMPILE) -c $< -o $@

.S.o:
		$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
		$(COMPILE) -S $< -o $@

flash:all
		$(AVRDUDE) -U flash:w:led_controller.hex:i
com:
		$(MINICOM)

clean:
		rm -f led_controller.hex led_controller.elf $(OBJECTS)

led_controller.elf: $(OBJECTS)
		$(COMPILE) -o led_controller.elf $(OBJECTS)

led_controller.hex: led_controller.elf
		rm -f led_controller.hex
		avr-objcopy -j .text -j .data -O ihex led_controller.elf led_controller.hex
		avr-size --format=avr --mcu=$(DEVICE) led_controller.elf


