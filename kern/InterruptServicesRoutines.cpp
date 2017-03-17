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
        struct gdt_descriptor* ds = (struct gdt_descriptor*) (GDT_BASE + (context->ds & 0xF8));
        u32 ds_base = ds->base0_15 + (ds->base16_23 << 16) + (ds->base24_31 << 24);

        char* msg = (char*) ((int)ds_base + (int)context->ebx);
        Screen::print("[userland] ");
        Screen::print(msg, SCREEN_YELLOW);
    } else {
        Screen::print("syscall ");
        Screen::print(sys_num);
        Screen::print(" not implemented\n");
    }

    return;
}