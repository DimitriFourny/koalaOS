#include "Memory.h"

void Memory::fill(char* destination, char value, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        destination[i] = value;
    }
}

void Memory::copy(char* destination, const char* source, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        destination[i] = source[i];
    }
}