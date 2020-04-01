#include "Keyboard.h"

#include "Screen.h"
#include "keyboardUS.h"

bool Keyboard::left_shift_ = false;
bool Keyboard::right_shift_ = false;

void Keyboard::ShowCharacter(unsigned char character) {
  switch (character & ~0x80) {
    case 0x29:
      left_shift_ = !(character & 0x80);
      break;
    case 0x35:
      right_shift_ = !(character & 0x80);
      break;
    default:
      if (character < 0x80) {
        const bool is_uppercase = left_shift_ || right_shift_;
        Screen::printCharacter(KEYBOARD[character * 4 + is_uppercase],
                               SCREEN_LIGHT_MAGENTA);
      }
  }
}