KERN=kernel
CXX=g++ -m32 -Wall -O2 -fno-stack-protector
SRC= $(filter-out kernel.cpp, $(wildcard *.cpp))
OBJ=kernel.o $(SRC:.cpp=.o) interruptions.o

all: $(KERN) 

kernel: $(OBJ)
	ld -m elf_i386 --oformat binary -Ttext 1000  $^ -o $@

.o: .cpp 
	$(CXX) -c $^

interruptions.o: interruptions.asm
	nasm -f elf -o $@ $^

clean:
	rm -f $(KERN) *.o
