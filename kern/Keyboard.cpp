#include "Keyboard.h"

#include "Screen.h"
#include "keyboardUS.h"

bool Keyboard::left_shift_ = false;
bool Keyboard::right_shift_ = false;

#define KEY_UP_BIT 0x80
#define KEY_CLR_UP_BIT(key) (key & ~KEY_UP_BIT)
#define KEY_IS_UP(key) (key & KEY_UP_BIT)
#define KEY_IS_DOWN(key) (!KEY_IS_UP(key))

void Keyboard::ShowCharacter(unsigned char character) {
  switch (KEY_CLR_UP_BIT(character)) {
    case 0x29:
      left_shift_ = KEY_IS_DOWN(character);
      break;
    case 0x35:
      right_shift_ = KEY_IS_DOWN(character);
      break;
    default:
      if (KEY_IS_DOWN(character)) {
        const bool is_uppercase = left_shift_ || right_shift_;
        Screen::printCharacter(KEYBOARD[character * 4 + is_uppercase],
                               SCREEN_LIGHT_MAGENTA);
      }
  }
}