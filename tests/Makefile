LIBDIR=/home/sam/z80em

CC=gcc
CFLAGS=-Wall -Wextra -g -O -std=gnu99 -I$(LIBDIR)
LDFLAGS=-lz80em -L$(LIBDIR) -lreadline

.PHONY: all
all: libs test

.PHONY: libs
libs:
	$(MAKE) -C $(LIBDIR) libz80em.so

.PHONY: clean
clean:
	rm -f test core *.o

test: main.o
	$(CC) -o $@ $< $(LDFLAGS) -Wl,-rpath,$(LIBDIR)
