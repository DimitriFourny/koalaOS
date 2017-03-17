#include "Keyboard.h"
#include "keyboardUS.h"
#include "Screen.h"

bool Keyboard::m_lshift = false;
bool Keyboard::m_rshift = false;

void Keyboard::showCharacter(unsigned char character) {    
    unsigned char c = character;
    if (c > 0x80) c -= 0x80;

    switch (c) {
        case 0x29:
            m_lshift = (c == character);
            break;
        case 0x35:
            m_rshift = (c == character);
            break;
        default:    
            if (character < 0x80) {
                Screen::printCharacter(KEYBOARD[character*4 + (m_lshift || m_rshift)], SCREEN_LIGHT_MAGENTA);
            }
    }
}