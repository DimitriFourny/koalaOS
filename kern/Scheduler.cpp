#include "Scheduler.h"

#include "GlobalDescriptorTable.h"
#include "Memory.h"
#include "Screen.h"

struct Process g_process[SCHEDULER_NB_PROCESS];
unsigned int g_nb_process = 0;
unsigned int g_current_pid = 0;

void Scheduler::newProcess(u32 function, u32 stack) {
  Memory::fill(reinterpret_cast<char*>(&g_process[g_nb_process]), 0,
               sizeof(g_process[g_nb_process]));

  g_process[g_nb_process].active = false;
  g_process[g_nb_process].pid = g_nb_process;
  g_process[g_nb_process].context.eip = function;
  g_process[g_nb_process].context.esp = stack;
  g_process[g_nb_process].context.cs =
      (GDT_SEGMENT_USER_CODE * sizeof(gdt_descriptor)) | RING3;
  g_process[g_nb_process].context.ds =
      (GDT_SEGMENT_USER_DATA * sizeof(gdt_descriptor)) | RING3;
  g_process[g_nb_process].context.ss =
      (GDT_SEGMENT_USER_STACK * sizeof(gdt_descriptor)) | RING3;
  g_nb_process++;
}

void Scheduler::schedule(Context* context) {
  // Save the context
  if (g_process[g_current_pid].active) {
    Memory::copy(reinterpret_cast<char*>(&(g_process[g_current_pid].context)),
                 reinterpret_cast<char*>(context),
                 sizeof(g_process[g_current_pid].context));
  }

  // Next PID
  g_current_pid = (g_current_pid + 1) % g_nb_process;
  if (!g_process[g_current_pid].active) {
    g_process[g_current_pid].active = true;
  }

  continueProcess(g_current_pid);
}

void Scheduler::continueProcess(unsigned int pid) {  // 0x1b96
  Context* context = &g_process[pid].context;
  context->eflags = (context->eflags | EFLAGS_INTERRUPT) & ~EFLAGS_NESTED_STACK;

  // Disable the interruptions, push the values for iret
  // iret = pop eip, cs, flags, esp, ss
  asm(" mov %5, %%esi     \n \
                            \n \
          push %0           \n \
          push %1           \n \
          push %2           \n \
          push %3           \n \
          push %4           \n \
                            \n \
          push 0(%%esi)     \n \
          push 4(%%esi)     \n \
          push 8(%%esi)     \n \
          push 12(%%esi)    \n \
          push 16(%%esi)    \n \
          push 20(%%esi)    \n \
          push 24(%%esi)    \n \
          push 28(%%esi)    \n \
          push 32(%%esi)    \n \
          push 36(%%esi)    \n \
          push 40(%%esi)    \n \
                            \n \
          mov $0x20, %%al   \n \
          out %%al, $0x20   \n \
                            \n \
          pop %%eax         \n \
          pop %%ecx         \n \
          pop %%edx         \n \
          pop %%ebx         \n \
          pop %%ebp         \n \
          pop %%esi         \n \
          pop %%edi         \n \
          pop %%ds          \n \
          pop %%es          \n \
          pop %%fs          \n \
          pop %%gs          \n \
                            \n \
          iret"
      :
      : "m"(context->ss), "m"(context->esp), "m"(context->eflags),
        "m"(context->cs), "m"(context->eip), "m"(context));
}
