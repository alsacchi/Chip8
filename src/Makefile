all: chip8
CC = clang++
CFLAG = -g -Wall
SDLLIB = `sdl2-config --cflags --libs`
chip8: emu.o chip8.o
	$(CC) $(CFLAG) $(SDLLIB) -o chip8 emu.o chip8.o 
chip8.o: chip8.cpp
	$(CC) $(CFLAG) -c chip8.cpp
emu.o: emu.cpp
	$(CC) $(CFLAG) -c emu.cpp
clean: 
	rm chip8 chip8.o emu.o
