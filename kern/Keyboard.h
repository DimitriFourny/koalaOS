#ifndef H_KEYBOARD
#define H_KEYBOARD

class Keyboard {
 public:
  static void ShowCharacter(unsigned char character);

 private:
  static bool left_shift_;
  static bool right_shift_;
};

#endif