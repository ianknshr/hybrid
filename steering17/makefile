
TARGET = steeringwheel.c

OUTPUT = steeringwheel.elf

build : 
	avr-gcc -mmcu=atmega328p -O1 $(TARGET) -o $(OUTPUT)

flash : build
	avrdude -p m328p -c avrisp -P /dev/ttyACM0 -b 19200 -U flash:w:$(OUTPUT)

fuse : 
	avrdude -p m328p -c avrisp -P /dev/ttyACM0 -b 19200 -U lfuse:w:0xE2:m
