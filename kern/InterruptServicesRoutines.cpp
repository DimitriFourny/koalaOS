#include "InterruptServicesRoutines.h"
#include "Screen.h"
#include "io.h"
#include "Keyboard.h"
#include "GlobalDescriptorTable.h"

void isr_default() {
    Screen::print("unknown interrupt\n");
}

void isr_clock() {
    /*static int tick = 0;
    tick = (tick+1) % 100;
    if (!tick) {
        Screen::print("clock\n");
    }*/
}

void isr_keyboard() {
    unsigned char character = inb(0x60);
    character--;
    Keyboard::showCharacter(character);
}

void do_syscall(int sys_num, struct Context* context) {
    if (sys_num == 1) {
        char* msg = reinterpret_cast<char*>(context->ebx);
        Screen::print("[userland] ");
        Screen::print(msg, SCREEN_YELLOW);
    } else {
        Screen::print("syscall ");
        Screen::print(sys_num);
        Screen::print(" not implemented\n");
    }

    return;
}