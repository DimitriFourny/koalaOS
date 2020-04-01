#ifndef H_MEMORY
#define H_MEMORY

class Memory {
 public:
  static void fill(char* destination, char value, unsigned int size);
  static void copy(char* destination, const char* source, unsigned int size);
};

#endif