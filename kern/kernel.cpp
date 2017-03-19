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

    // Update the stack to point to the GDT
    asm("movw %0, %%ax     \n \
         movw %%ax, %%ss   \n \
         movl %1, %%esp"            
        : 
        : "i"((GDT_SEGMENT_STACK*sizeof(gdt_descriptor))), 
          "i"(GDT_KERNEL_STACK)
    );

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
    Memory::copy((char*) GDT_USERLAND_LINEAR_BASE, (char*) &userTask, 100);
    callUserFunction(GDT_USERLAND_LINEAR_BASE, GDT_USERLAND_LINEAR_STACK);

    Screen::print("Userland task finished :)");
    while (1);
}

