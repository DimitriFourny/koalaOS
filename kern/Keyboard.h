#ifndef H_KEYBOARD
#define H_KEYBOARD

class Keyboard {
 public:
  static void showCharacter(unsigned char character);

 private:
  static bool m_lshift;
  static bool m_rshift;
};

#endif