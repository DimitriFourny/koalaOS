#include "InterruptServicesRoutines.h"
#include "Screen.h"
#include "io.h"
#include "Keyboard.h"
#include "GlobalDescriptorTable.h"
#include "Scheduler.h"

unsigned int g_tick = 0;
unsigned int g_sec = 0;

void isr_default() {
    Screen::print("unknown interrupt\n");
}

void isr_clock(struct Context* context) {
    g_tick++;
    if (g_tick % 100 == 0) {
        g_sec++;
    }

    Scheduler::schedule(context);
}

void isr_keyboard() {
    unsigned char character = inb(0x60);
    character--;
    Keyboard::showCharacter(character);
}

void do_syscall(int sys_num, struct Context* context) {
    if (sys_num == 1) {                                    // print
        char* msg = reinterpret_cast<char*>(context->ebx);
        Screen::print("[userland] ");
        Screen::print(msg, SCREEN_YELLOW);
    } else if (sys_num == 2) {                             // get_uptime
        unsigned int* uptime = reinterpret_cast<unsigned int*>(context->ebx);
        *uptime = g_tick;
    } else {
        Screen::print("syscall ");
        Screen::print(sys_num);
        Screen::print(" not implemented\n");
    }

    return;
}