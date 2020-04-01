#ifndef H_INTERRUPTCONTROLLER
#define H_INTERRUPTCONTROLLER

#include "io.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

#define PIC_IRQ_MASTER 0x20  // IRQ 0-7
#define PIC_IRQ_SLAVE 0x70   // IRQ 8-15

class InterruptController {
 public:
  static void init();
};

#endif