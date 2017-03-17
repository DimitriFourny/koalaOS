#define __IDT__
#include "InterruptDescriptorTable.h"

struct idt_descriptor g_idt_descriptors[IDT_SIZE]; 
struct idtr g_idtr; 

void InterruptDescriptorTable::initDescriptor(u16 selector, u32 offset, u8 type, struct idt_descriptor* desc) {
    desc->offset0_15  = (offset & 0xffff);
    desc->selector    = selector;
    desc->zero        = 0;
    desc->type        = type;
    desc->offset16_31 = (offset & 0xffff0000) >> 16;
}

void InterruptDescriptorTable::init() {
    u16 selector_code = GDT_SEGMENT_CODE * sizeof(struct gdt_descriptor);

    for (unsigned i = 0; i < IDT_SIZE; i++) {
        initDescriptor(selector_code, (u32) asm_int_default, IDT_INTERRUPT_GATE, &g_idt_descriptors[i]);
    }
    initDescriptor(selector_code, (u32) asm_irq0_clock,    IDT_INTERRUPT_GATE, &g_idt_descriptors[IDT_IRQ0_CLOCK]);    // clock 
    initDescriptor(selector_code, (u32) asm_irq1_keyboard, IDT_INTERRUPT_GATE, &g_idt_descriptors[IDT_IRQ1_KEYBOARD]); // keyboard
    initDescriptor(selector_code, (u32) asm_syscall,       IDT_TRAP_GATE,      &g_idt_descriptors[IDT_SYSCALL]);       // syscall

    g_idtr.size = (IDT_SIZE * sizeof(struct idt_descriptor))-1;
    g_idtr.offset = IDT_BASE;
    Memory::copy((char*) g_idtr.offset, (char*) g_idt_descriptors, g_idtr.size+1);

    asm("lidtl (g_idtr)");
}

void InterruptDescriptorTable::enable() {
    sti;
}