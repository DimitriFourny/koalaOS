#include "userland.h"

void userTask1() {
  char* msg = (char*)0x1000000 + 0x100;
  msg[0] = 'T';
  msg[1] = 'a';
  msg[2] = 's';
  msg[3] = 'k';
  msg[4] = ' ';
  msg[5] = '1';
  msg[6] = '\n';
  msg[7] = '\0';

  unsigned int uptime = 0;
  unsigned int sleep = 0;
  unsigned int* pUptime = &uptime;

  while (1) {
    asm("mov %0, %%ebx    \n\
             mov $0x02, %%eax \n\
             int $0x30" ::"m"(pUptime));

    if ((uptime - sleep) > 100) {  // every second
      sleep = uptime;

      asm("mov %0, %%ebx    \n\
                 mov $0x01, %%eax \n\
                 int $0x30" ::"m"(msg));
    }

    for (unsigned int i = 0; i < 1000000; i++)
      ;
  }

  while (1)
    ;
}

void userTask2() {
  char* msg = (char*)0x1000000 + 0x200;
  msg[0] = 'T';
  msg[1] = 'a';
  msg[2] = 's';
  msg[3] = 'k';
  msg[4] = ' ';
  msg[5] = '2';
  msg[6] = '\n';
  msg[7] = '\0';

  unsigned int uptime = 0;
  unsigned int sleep = 0;
  unsigned int* pUptime = &uptime;

  while (1) {
    asm("mov %0, %%ebx    \n\
             mov $0x02, %%eax \n\
             int $0x30" ::"m"(pUptime));

    if ((uptime - sleep) > 100) {  // every second
      sleep = uptime;

      asm("mov %0, %%ebx    \n\
                 mov $0x01, %%eax \n\
                 int $0x30" ::"m"(msg));
    }

    for (unsigned int i = 0; i < 1000000; i++)
      ;
  }

  while (1)
    ;
}