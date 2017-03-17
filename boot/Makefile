OBJ=bootloader 

all: $(OBJ) 

bootloader: bootloader.asm
	nasm -f bin -o $@ $^ ;

clean:
	rm -f $(OBJ) *.o

