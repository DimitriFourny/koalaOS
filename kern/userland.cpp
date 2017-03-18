#include "userland.h"

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