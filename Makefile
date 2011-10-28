SONAME=libz80em.so
EXEC=z80em
DIR=/home/sam/z80em

SODEPS:=setup.o machine.o cpu.o dbg.o dbg_cmds.o
EDEPS=main.o

CC=gcc
CFLAGS=-Wall -Wextra -g -O -std=gnu99
LDFLAGS=-lz80em -L$(DIR) -lreadline

.PHONY: all
all: $(SONAME) $(EXEC)

.PHONY: clean
clean:
	rm -f $(SONAME) $(EXEC) core *.o

$(SONAME): $(SODEPS)
	$(CC) -shared -Wl,-soname,$@ -o $@ $^

%.o: %.c
	$(CC) -fPIC $(CFLAGS) -c $<

z80em: $(EDEPS) $(SONAME)
	$(CC) -o $@ $< $(LDFLAGS) -Wl,-rpath,$(DIR)
