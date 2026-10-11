ifeq ($(origin CXX),default)
CXX = clang++
endif
ifeq ($(origin LD),default)
LD = ld.lld
ifeq ($(shell uname -s),Darwin)
LD = $(shell brew --prefix lld)/bin/ld.lld
endif
endif
NASM ?= nasm

CPPFLAGS += -MMD -MP
# Keep O0: the original interrupt code and copied user tasks assume this layout.
CXXFLAGS = --target=i386-unknown-none-elf -std=c++11 -O0 -Wall \
	-ffreestanding -fno-exceptions -fno-rtti -fno-stack-protector \
	-fno-pic -fno-pie -fno-unwind-tables -fno-asynchronous-unwind-tables \
	-mno-sse -mno-sse2 -mno-mmx
SRC = $(wildcard *.cpp)
OBJ = start.o $(SRC:.cpp=.o) interruptions.o

.PHONY: all clean
.DELETE_ON_ERROR:

all: kernel

kernel: $(OBJ) linker.ld
	$(LD) -m elf_i386 --oformat binary -T linker.ld $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.asm
	$(NASM) -f elf32 -o $@ $<

clean:
	rm -f kernel *.o *.d

-include $(SRC:.cpp=.d)
