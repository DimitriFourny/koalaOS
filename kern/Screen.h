#ifndef H_SCREEN
#define H_SCREEN

#define SCREEN_RAM_ADDR 0xB8000
#define SCREEN_SIZE 0xFA0
#define SCREEN_RAM_LIMIT (SCREEN_RAM_ADDR + SCREEN_SIZE)
#define SCREEN_TAB_SIZE 8
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_LINE_SIZE (2 * SCREEN_WIDTH)  // 2 bytes per character

enum SCREEN_COLOR {
  SCREEN_BLACK,
  SCREEN_BLUE,
  SCREEN_GREEN,
  SCREEN_CYAN,
  SCREEN_RED,
  SCREEN_MAGENTA,
  SCREEN_BROWN,
  SCREEN_LIGHT_GRAY,
  SCREEN_DARK_GRAY,
  SCREEN_LIGHT_BLUE,
  SCREEN_LIGHT_GREEN,
  SCREEN_LIGHT_CYAN,
  SCREEN_LIGHT_RED,
  SCREEN_LIGHT_MAGENTA,
  SCREEN_YELLOW,
  SCREEN_WHITE
};

class Screen {
 public:
  static void init(char x = 0, char y = 0, char attr = SCREEN_WHITE);
  static void clear();
  static void scrollUp(unsigned int n);
  static void printCharacter(unsigned char c, unsigned char attributes = 0);
  static void print(const char* string, unsigned char attributes = 0);
  static void print(int value, unsigned char attributes = 0);

 private:
  static char m_x;
  static char m_y;
  static char m_attr;
};

#endif