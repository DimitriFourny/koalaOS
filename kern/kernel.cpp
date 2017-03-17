#include "Screen.h"
#include "GlobalDescriptorTable.h"
#include "InterruptDescriptorTable.h"
#include "InterruptController.h"
#include "InterruptServicesRoutines.h"
#include "userland.h"

extern "C" void _start() {
    Screen::init();
    Screen::clear();

    Screen::print("=== KoalaOS ===\n", SCREEN_LIGHT_GREEN);
    Screen::print("Kernel loaded!\n");

    GlobalDescriptorTable::init();
    Screen::print("GDT loaded!\n");

    InterruptDescriptorTable::init();
    Screen::print("IDT loaded!\n");

    InterruptController::init();
    InterruptDescriptorTable::enable();
    Screen::print("PIC initialized!\n");

    Screen::print("Launch userland task...\n\n");
    Memory::copy((char*) GDT_USERSPACE_BASE, (char*) &userTask, 100);
    g_tss_default.esp0 = GDT_USERLAND_STACK;

    asm("   cli                    \n \
            push %0                \n \
            push %1                \n \
            pushfl                 \n \
            popl %%eax             \n \
            orl $0x200, %%eax      \n \
            and $0xffffbfff, %%eax \n \
            push %%eax             \n \
            push %2             \n \
            push $0x0              \n \
            movw %3, %%ax       \n \
            movw %%ax, %%ds        \n \
            iret" 
            : 
            : "i"((GDT_SEGMENT_USER_STACK*sizeof(gdt_descriptor)) | RING3), 
              "i"(GDT_USERSPACE_BASE),
              "i"((GDT_SEGMENT_USER_CODE*sizeof(gdt_descriptor)) | RING3),
              "i"((GDT_SEGMENT_USER_DATA*sizeof(gdt_descriptor)) | RING3)
    );

    Screen::print("Userland task finished :)");
    while (1);
}

