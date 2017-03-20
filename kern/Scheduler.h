#ifndef H_SCHEDULER
#define H_SCHEDULER

#include "types.h"

#define SCHEDULER_NB_PROCESS 32
#define EFLAGS_INTERRUPT    (1 << 9)
#define EFLAGS_NESTED_STACK (1 << 14)

struct Context {
    u32 gs, fs, es, ds;
    u32 edi, esi;
    u32 ebp, unk;
    u32 ebx, edx, ecx, eax;
    u32 eip, cs, eflags;
    u32 esp, ss;
} __attribute__ ((packed));

struct Process {
    bool active;
    unsigned int pid;
    Context context;
} __attribute__ ((packed));

class Scheduler {
public:
    static void newProcess(u32 function, u32 stack);
    static void schedule(Context* context);
private:
    static void continueProcess(unsigned int pid);
};

#endif