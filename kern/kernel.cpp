#include "Screen.h"
#include "GlobalDescriptorTable.h"
#include "InterruptDescriptorTable.h"
#include "InterruptController.h"
#include "InterruptServicesRoutines.h"
#include "Paging.h"
#include "userland.h"

extern "C" void _start() {
    Screen::init();
    Screen::clear();

    Screen::print("=== KoalaOS ===\n", SCREEN_LIGHT_GREEN);
    Screen::print("Kernel loaded!\n");

    Screen::print("Loading GDT");
    GlobalDescriptorTable::init();
    Screen::print("\t\tOK\n", SCREEN_LIGHT_GREEN);

    Screen::print("Loading IDT");
    InterruptDescriptorTable::init();
    Screen::print("\t\tOK\n", SCREEN_LIGHT_GREEN);

    Screen::print("PIC Initialization");
    InterruptController::init();
    InterruptDescriptorTable::enable();
    Screen::print("\tOK\n", SCREEN_LIGHT_GREEN);

    Screen::print("Paging Initialization");
    Paging::init();
    Paging::enable();
    Screen::print("\tOK\n", SCREEN_LIGHT_GREEN);

    Screen::print("Launch userland task...\n\n");
    Memory::copy((char*) GDT_USERLAND_BASE, (char*) &userTask, 100);
    g_tss_default.esp0 = GDT_USERLAND_STACK;

    asm("   cli                    \n \
            push %0                \n \
            push %1                \n \
            pushfl                 \n \
            popl %%eax             \n \
            orl $0x200, %%eax      \n \
            and $0xffffbfff, %%eax \n \
            push %%eax             \n \
            push %2                \n \
            push %3                \n \
            movw %4, %%ax          \n \
            movw %%ax, %%ds        \n \
            iret" 
            : 
            : "i"((GDT_SEGMENT_USER_STACK*sizeof(gdt_descriptor)) | RING3), 
              "i"(GDT_USERLAND_STACK),
              "i"((GDT_SEGMENT_USER_CODE*sizeof(gdt_descriptor)) | RING3),
              "i"(GDT_USERLAND_BASE),
              "i"((GDT_SEGMENT_USER_DATA*sizeof(gdt_descriptor)) | RING3)
    );

    Screen::print("Userland task finished :)");
    while (1);
}

