CC=gcc
CFLAGS=-Wall -Wextra -g -O -std=gnu99

.PHONY: all
all: z80em

.PHONY: clean
clean:
	rm -f z80em core *.o

z80em: main.o machine.o cpu.o
	$(CC) -o $@ $^
