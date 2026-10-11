QEMU ?= qemu-system-i386
QEMU_FLAGS ?=

.PHONY: all bootloader kernel run clean
.DELETE_ON_ERROR:

all: floppyA

floppyA: bootloader kernel
	dd if=/dev/zero of=$@ bs=512 count=2880
	cat boot/bootloader kern/kernel | dd of=$@ bs=512 conv=notrunc

bootloader:
	$(MAKE) -C boot

kernel:
	$(MAKE) -C kern

run: floppyA
	$(QEMU) -machine pc -accel tcg -m 32 -boot order=a \
		-drive file=floppyA,format=raw,if=floppy,readonly=on \
		-no-reboot $(QEMU_FLAGS)

clean:
	rm -f floppyA
	$(MAKE) -C boot clean
	$(MAKE) -C kern clean
