#ifndef H_INTERRUPTSERVICESROUTINES
#define H_INTERRUPTSERVICESROUTINES

#include "Scheduler.h"

extern "C" {
void isr_default();
void isr_clock(struct Context* context);
void isr_keyboard();
void do_syscall(int sys_num, struct Context* context);
}

#endif