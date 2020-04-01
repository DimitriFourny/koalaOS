#ifndef H_INTERRUPTDESCRIPTORTABLE
#define H_INTERRUPTDESCRIPTORTABLE

#include "GlobalDescriptorTable.h"
#include "InterruptController.h"
#include "Memory.h"
#include "interruptions.h"
#include "types.h"

#define IDT_BASE 0x800
#define IDT_SIZE 0x100
#define IDT_INTERRUPT_GATE 0x8E
#define IDT_TRAP_GATE 0xEF
#define IDT_IRQ0_CLOCK PIC_IRQ_MASTER
#define IDT_IRQ1_KEYBOARD (PIC_IRQ_MASTER + 1)
#define IDT_SYSCALL 0x30

struct idt_descriptor {
  u16 offset0_15;
  u16 selector;
  u8 zero;
  u8 type;
  u16 offset16_31;
} __attribute__((packed));

struct idtr {
  u16 size;
  u32 offset;
} __attribute__((packed));

extern struct idt_descriptor g_idt_descriptors[IDT_SIZE];
extern struct idtr g_idtr;

class InterruptDescriptorTable {
 public:
  static void init();
  static void enable();

 private:
  static void initDescriptor(u16 selector,
                             u32 offset,
                             u8 type,
                             struct idt_descriptor* desc);
};

#endif