#include "Screen.h"

char Screen::m_x = 0;           
char Screen::m_y = 0;           
char Screen::m_attr = SCREEN_WHITE;           

void Screen::init(char x, char y, char attr) {
    m_x = x;
    m_y = y;
    m_attr = attr;
}

void Screen::clear() {
    unsigned char* video = reinterpret_cast<unsigned char*>(SCREEN_RAM_ADDR);

    for (unsigned int i = 0; i < SCREEN_SIZE; i++) {
        video[i] = 0;
    }
}

void Screen::scrollUp(unsigned int n) {
    unsigned char* video = (unsigned char*) SCREEN_RAM_ADDR;
 
    // Move all lines to up
    for (unsigned int i = 0; i < SCREEN_SIZE-SCREEN_LINE_SIZE; i++) {
        video[i] = video[i + SCREEN_LINE_SIZE];
    }

    // The last line is empty
    for (unsigned int i = SCREEN_SIZE-SCREEN_LINE_SIZE; i < SCREEN_SIZE; i++) {
        video[i] = 0;
    }

    m_y -= n;
    if (m_y < 0) {
        m_y = 0;
    }
}

void Screen::printCharacter(unsigned char c, unsigned char attributes) {
    if (attributes == 0) {
        attributes = m_attr;
    }

    if (c == '\n') { 
        m_x = 0;
        m_y++;
    } else if (c == '\t') {
        m_x = m_x + SCREEN_TAB_SIZE - (m_x % SCREEN_TAB_SIZE);
    } else if (c == '\r') { 
        m_x = 0;
    } else {  
        unsigned char* video = reinterpret_cast<unsigned char*>(SCREEN_RAM_ADDR + 2 * m_x + 2*SCREEN_WIDTH*m_y);
        video[0] = c;
        video[1] = attributes;

        m_x++;
        if (m_x >= SCREEN_WIDTH) {
            m_x = 0;
            m_y++;
        }
    }

    if (m_y >= SCREEN_HEIGHT) {
        scrollUp(m_y - SCREEN_HEIGHT);
    }
}

void Screen::print(const char* string, unsigned char attributes) {
    while (*string != 0) {    
        printCharacter(*string, attributes);
        string++;
    }
}

void Screen::print(int value, unsigned char attributes) {
    char str[20] = {0};
    int tmpValue;
    unsigned int len = 0;
    bool isNegative = false;

    // Check if negative
    if (value < 0) {
        isNegative = true;
        value = -value;
    }

    // Get all digits
    do {
        tmpValue = value / 10;
        tmpValue = value - tmpValue * 10;
        str[len] = tmpValue + '0';

        len++;
        if (len >= sizeof(str)) {
            break;
        }

        value /= 10;
    } while (value != 0);
    str[len] = '\0';

    if (isNegative) {
        str[len++] = '-';
    }

    // Reverse the string
    for (unsigned int i = 0; i < len / 2; i++) {
        tmpValue = str[i];
        str[i] = str[len-1-i];
        str[len-1-i] = tmpValue;
    }
    
    print(str, attributes);
}