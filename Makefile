OBJ=floppyA 

all: $(OBJ) 

floppyA: bootloader kernel
	cat boot/bootloader kern/kernel /dev/zero | dd of=floppyA bs=512 count=2880

bootloader: 
	make -C boot

kernel: 
	make -C kern

clean:
	rm -f $(OBJ) *.o
	make -C boot clean
	make -C kern clean
