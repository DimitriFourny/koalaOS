#ifndef H_INTERRUPTIONS
#define H_INTERRUPTIONS

struct Context;

extern "C" {
void asm_int_default();
void asm_irq0_clock();
void asm_irq1_keyboard();
void asm_syscall();
[[noreturn]] void asm_resume_process(Context* context);
}

#endif