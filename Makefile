# File:   Makefile
# Author: Harrison Cook, hgc25
# Date:   11 October 2018
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I./utils -I./drivers -I./drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ./drivers/avr/system.h ./utils/tinygl.h ./drivers/display.h ./fonts/font3x5_1.h ./utils/font.h ./drivers/avr/timer.h ./drivers/navswitch.h lights.h setupGame.h shoot.h
	$(CC) -c $(CFLAGS) $< -o $@

lights.o: lights.c ./drivers/avr/system.h ./utils/tinygl.h lights.h
	$(CC) -c $(CFLAGS) $< -o $@

setupGame.o: setupGame.c setupGame.h ./drivers/avr/system.h ./drivers/avr/system.h ./utils/tinygl.h ./drivers/display.h ./drivers/avr/pio.h lights.h
	$(CC) -c $(CFLAGS) $< -o $@

shoot.o: shoot.c ./drivers/avr/system.h ./drivers/avr/system.h ./utils/tinygl.h ./drivers/display.h lights.h
		$(CC) -c $(CFLAGS) $< -o $@

system.o: ./drivers/avr/system.c ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ./utils/pacer.c ./drivers/avr/system.h ./drivers/avr/timer.h ./utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ./utils/tinygl.c ./drivers/avr/system.h ./drivers/display.h ./utils/font.h ./utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@


pio.o: ./drivers/avr/pio.c ./drivers/avr/pio.h ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ./drivers/avr/timer.c ./drivers/avr/system.h ./drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

button.o: ./drivers/button.c ./drivers/avr/pio.h ./drivers/avr/system.h ./drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ./drivers/display.c ./drivers/avr/system.h ./drivers/display.h ./drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ./drivers/ledmat.c ./drivers/avr/pio.h ./drivers/avr/system.h ./drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ./utils/font.c ./drivers/avr/system.h ./utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ./drivers/avr/ir_uart.h ./drivers/avr/pio.h ./drivers/avr/system.h ./drivers/avr/timer0.h ./drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ./drivers/avr/prescale.c ./drivers/avr/prescale.h ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ./drivers/avr/timer0.c ./drivers/avr/bits.h ./drivers/avr/prescale.h ./drivers/avr/system.h ./drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ./drivers/avr/usart1.c ./drivers/avr/system.h ./drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ./drivers/navswitch.c ./drivers/avr/delay.h ./drivers/avr/pio.h ./drivers/avr/system.h ./drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ./utils/task.c ./drivers/avr/system.h ./drivers/avr/timer.h ./utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@



# Link: create ELF output file from object files.
game.out: game.o lights.o setupGame.o shoot.o pio.o system.o timer.o button.o display.o ledmat.o font.o pacer.o tinygl.o ir_uart.o prescale.o timer0.o usart1.o navswitch.o task.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


#
