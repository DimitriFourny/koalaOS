#ifndef H_INTERRUPTIONS
#define H_INTERRUPTIONS

extern "C" {
void asm_int_default();
void asm_irq0_clock();
void asm_irq1_keyboard();
void asm_syscall();
}

#endif