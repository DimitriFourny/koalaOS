#include "userland.h"

#define EFLAGS_INTERRUPT    (1 << 9)
#define EFLAGS_NESTED_STACK (1 << 14)

void callUserFunction(u32 function, u32 stack) {
    // We will manipulate the stack, so we need to save the parameters
    // EBX = stack
    // ECX = function
    asm(" mov %0, %%ebx          \n \
          mov %1, %%ecx" 
          : 
          : "m"(stack),
            "m"(function)
    );

    // Prepare the EFLAGS in EAX
    asm(" pushfl                 \n \
          popl %%eax             \n \
          orl %0, %%eax          \n \
          and %1, %%eax" 
          : 
          : "i"(EFLAGS_INTERRUPT),
            "i"(~EFLAGS_NESTED_STACK)
    );

    // Disable the interruptions and jump into ring3
    asm(" cli                    \n \
          push %0                \n \
          push %%ebx             \n \
          push %%eax             \n \
          push %1                \n \
          push %%ecx             \n \
          movw %2, %%ax          \n \
          movw %%ax, %%ds        \n \
          iret" 
          : 
          : "i"((GDT_SEGMENT_USER_STACK*sizeof(gdt_descriptor)) | RING3), 
            "i"((GDT_SEGMENT_USER_CODE*sizeof(gdt_descriptor)) | RING3),
            "i"((GDT_SEGMENT_USER_DATA*sizeof(gdt_descriptor)) | RING3)
    );
}


void userTask() {
    char* msg = (char*) 0x1000000 + 0x100;
    msg[0] = 'H';
    msg[1] = 'e';
    msg[2] = 'l';
    msg[3] = 'l';
    msg[4] = 'o';
    msg[5] = '\n';
    msg[6] = '\0';

    asm("mov %0, %%ebx    \n\
         mov $0x01, %%eax \n\
         int $0x30" 
         :: "m" (msg));

    asm("mov $0x02, %eax \n\
         int $0x30");
    
    while (1);
}