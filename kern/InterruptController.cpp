#include "InterruptController.h"

void InterruptController::init() {
    // ICW1
    outbp(PIC1_COMMAND, ICW1_ICW4|ICW1_INIT); 
    outbp(PIC2_COMMAND, ICW1_ICW4|ICW1_INIT); 

    // ICW2
    outbp(PIC1_DATA, PIC_IRQ_MASTER);
    outbp(PIC2_DATA, PIC_IRQ_SLAVE); 

    // ICW3
    outbp(PIC1_DATA, 0x04); // Slave PIC at IRQ2
    outbp(PIC2_DATA, 0x02); // IRQ2

    // ICW4
    outbp(PIC1_DATA, ICW4_8086); 
    outbp(PIC2_DATA, ICW4_8086); 

    outb(PIC1_DATA, 0x0); 
    outb(PIC2_DATA, 0x0); 
}