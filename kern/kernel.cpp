#include "GlobalDescriptorTable.h"
#include "InterruptController.h"
#include "InterruptDescriptorTable.h"
#include "InterruptServicesRoutines.h"
#include "Paging.h"
#include "Scheduler.h"
#include "Screen.h"
#include "userland.h"

extern "C" void kernel_main() {
  Screen::init();
  Screen::clear();

  Screen::print("=== KoalaOS ===\n", SCREEN_LIGHT_GREEN);
  Screen::print("Kernel loaded!\n");

  Screen::print("Loading GDT");
  GlobalDescriptorTable::init();
  Screen::print("\t\t\tOK\n", SCREEN_LIGHT_GREEN);

  // The assembly entry point has already set up the kernel stack.
  asm volatile("movw %0, %%ax\n"
               "movw %%ax, %%ss"
               :
               : "i"(GDT_SEGMENT_STACK * sizeof(gdt_descriptor))
               : "eax");

  Screen::print("Loading IDT");
  InterruptDescriptorTable::init();
  Screen::print("\t\t\tOK\n", SCREEN_LIGHT_GREEN);

  Screen::print("PIC Initialization");
  InterruptController::init();
  Screen::print("\t\tOK\n", SCREEN_LIGHT_GREEN);

  Screen::print("Paging Initialization");
  Paging::init();
  Paging::enable();
  Screen::print("\t\tOK\n", SCREEN_LIGHT_GREEN);

  Screen::print("Launch userland task 1");
  Memory::copy((char*)GDT_USERLAND_LINEAR_BASE, (char*)&userTask1, 500);
  Scheduler::newProcess(GDT_USERLAND_LINEAR_BASE, GDT_USERLAND_LINEAR_STACK);
  Screen::print("\t\tOK\n", SCREEN_LIGHT_GREEN);

  Screen::print("Launch userland task 2");
  Memory::copy((char*)GDT_USERLAND_LINEAR_BASE + 0x10000, (char*)&userTask2,
               500);
  Scheduler::newProcess(GDT_USERLAND_LINEAR_BASE + 0x10000,
                        GDT_USERLAND_LINEAR_STACK + 0x10000);
  Screen::print("\t\tOK\n\n", SCREEN_LIGHT_GREEN);

  // Timer interrupts schedule tasks, so enable them only once tasks exist.
  InterruptDescriptorTable::enable();

  while (1)
    ;
}
