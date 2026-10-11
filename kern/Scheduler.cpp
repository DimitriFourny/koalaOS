#include "Scheduler.h"

#include "GlobalDescriptorTable.h"
#include "Memory.h"
#include "Screen.h"
#include "interruptions.h"

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

void Scheduler::continueProcess(unsigned int pid) {
  Context* context = &g_process[pid].context;
  context->eflags = (context->eflags | EFLAGS_INTERRUPT) & ~EFLAGS_NESTED_STACK;
  asm_resume_process(context);
}
