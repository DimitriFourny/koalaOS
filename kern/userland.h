#ifndef H_USERLAND
#define H_USERLAND

#include "types.h"
#include "GlobalDescriptorTable.h"

void userTask();
void callUserFunction(u32 function, u32 stack);

#endif