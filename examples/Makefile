EXAMPLES=nop.bin strlen.bin toggle.bin

AS=~/Downloads/z80asm-1.8/z80asm -l -L

.PHONY: all
all: $(EXAMPLES)

.PHONY: clean
clean:
	rm -f *.bin

%.bin: %.asm
	$(AS) -o $@ $<
