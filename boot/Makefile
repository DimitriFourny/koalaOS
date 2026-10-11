NASM ?= nasm

.PHONY: all clean

all: bootloader

bootloader: bootloader.asm
	$(NASM) -f bin -o $@ $<

clean:
	rm -f bootloader
