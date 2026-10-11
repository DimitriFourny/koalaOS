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

// Clang can emit memset for aggregate initialization, even in freestanding code.
extern "C" void* memset(void* destination, int value, __SIZE_TYPE__ size) {
  Memory::fill(static_cast<char*>(destination), static_cast<char>(value), size);
  return destination;
}