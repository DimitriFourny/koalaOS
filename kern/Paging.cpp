#include "Paging.h"
#include "GlobalDescriptorTable.h"

#define TABLE_USED 1
#define TABLE_FREE 0
#define BITMAP_SIZE (PAGING_PAGE_NB_LINE / sizeof(u8))
u8 g_page_bitmap[BITMAP_SIZE];

u32 Paging::getFreeTable() {
    u8 byte = 0;
    u8 bit = 0;

    for (byte = 0; byte < BITMAP_SIZE; byte++) {
        if (g_page_bitmap[byte] != 0xFF) {
            for (bit = 0; bit < 8; bit++) {
                if ((g_page_bitmap[byte] & (1 << bit)) == TABLE_FREE) {
                    g_page_bitmap[byte] |= (TABLE_USED << bit); 
                    return PAGING_PAGE_TABLE + (byte*8 + bit)*PAGING_PAGE_TABLE_SIZE;
                }
            }
        }
    }

    return 0;
}

void Paging::init() {
    // Initialize bitmap
    for (unsigned int i = 0; i < BITMAP_SIZE; i++) {
        g_page_bitmap[i] = TABLE_FREE;
    }

    // Page Directory
    u32* page_directory = reinterpret_cast<u32*>(PAGING_PAGE_DIRECTORY);
    for (unsigned int i = 0; i < PAGING_PAGE_NB_LINE; i++) {
        page_directory[i] = 0;
    }

    // Kernel Page Table
    u32* page_table = reinterpret_cast<u32*>(getFreeTable());
    page_directory[0] = reinterpret_cast<u32>(page_table) | PAGING_PAGE_READWRITE | PAGING_PAGE_ENABLE;
    for (unsigned int i = 0; i < PAGING_PAGE_NB_LINE; i++) {
        page_table[i] = (i * PAGING_PAGE_ENTRY_SIZE) | PAGING_PAGE_READWRITE | PAGING_PAGE_ENABLE;
    }

    // Userland Page 
    page_table = reinterpret_cast<u32*>(getFreeTable());
    unsigned int index = PAGING_ADDR_TO_PAGE(GDT_USERLAND_LINEAR_BASE);
    page_directory[index] = reinterpret_cast<u32>(page_table) | PAGING_PAGE_USER | PAGING_PAGE_READWRITE | PAGING_PAGE_ENABLE; 
    for (unsigned int i = 0; i < PAGING_PAGE_NB_LINE; i++) {
        page_table[i] = (GDT_USERLAND_PHYSICAL_BASE + i*PAGING_PAGE_ENTRY_SIZE) | PAGING_PAGE_USER | PAGING_PAGE_READWRITE | PAGING_PAGE_ENABLE;
    }

    asm("mov %0, %%eax \n \
         mov %%eax, %%cr3" :: "i"(PAGING_PAGE_DIRECTORY));
}

void Paging::enable() {
    asm("mov %%cr0, %%eax \n \
         or %0, %%eax     \n \
         mov %%eax, %%cr0" :: "i"(PAGING_CR0_FLAG)); 
}