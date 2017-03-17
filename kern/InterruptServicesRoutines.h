#ifndef H_INTERRUPTSERVICESROUTINES
#define H_INTERRUPTSERVICESROUTINES

#include "types.h"

struct Context {
    u32 gs, fs, es, ds;
    u32 edi, esi;
    u32 ebp, esp;
    u32 ebx, edx, ecx, eax;
} __attribute__ ((packed));


extern "C" {
    void isr_default();
    void isr_clock();
    void isr_keyboard();
    void do_syscall(int sys_num, struct Context* context);
}

#endif 