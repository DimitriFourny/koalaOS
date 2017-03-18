#include "Paging.h"
#include "types.h"
#include "GlobalDescriptorTable.h"

void Paging::init() {
    // Page Directory
    u32* page_directory = reinterpret_cast<u32*>(PAGING_PAGE_DIRECTORY);
    page_directory[0] = PAGING_PAGE_TABLE | PAGING_PAGE_READWRITE | PAGING_PAGE_ENABLE;
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0;
    }

    // Page Table
    u32* page_table = reinterpret_cast<u32*>(PAGING_PAGE_TABLE);
    for (int i = 0; i < 512; i++) {
        page_table[i] = (i * PAGING_PAGE_ENTRY_SIZE) | PAGING_PAGE_READWRITE | PAGING_PAGE_ENABLE;
    }
    for (int i = 512; i < 1024; i++) {
        page_table[i] = 0;
    }

    // Userland
    page_directory[GDT_USERSPACE_BASE >> 22] = (PAGING_PAGE_TABLE + 1024*sizeof(u32)) | PAGING_PAGE_USER | PAGING_PAGE_READWRITE | PAGING_PAGE_ENABLE; 
    page_table = reinterpret_cast<u32*>(PAGING_PAGE_TABLE + 1024*sizeof(u32));
    for (int i = 0; i < 1024; i++) {
        page_table[i] = (0x200000 + i*PAGING_PAGE_ENTRY_SIZE) | PAGING_PAGE_USER | PAGING_PAGE_READWRITE | PAGING_PAGE_ENABLE;
    }

    asm("mov %0, %%eax \n \
         mov %%eax, %%cr3" :: "i"(PAGING_PAGE_DIRECTORY));
}

void Paging::enable() {
    asm("mov %%cr0, %%eax \n \
         or %0, %%eax     \n \
         mov %%eax, %%cr0" :: "i"(PAGING_CR0_FLAG)); 
}