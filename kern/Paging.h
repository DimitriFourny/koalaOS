#ifndef H_PAGING
#define H_PAGING

#include "types.h"

#define PAGING_PAGE_DIRECTORY  0x100000     // required size : 1024*4      = 0x1000
#define PAGING_PAGE_TABLE      0x101000     // required size : 1024*1024*4 = 0x400000
#define PAGING_PAGE_NB_LINE    1024
#define PAGING_PAGE_ENTRY_SIZE 4096         // 4kb
#define PAGING_PAGE_TABLE_SIZE (PAGING_PAGE_NB_LINE*4)
#define PAGING_CR0_FLAG        0x80000000
#define PAGING_ADDR_TO_PAGE(x) (x >> 22)

#define PAGING_PAGE_ENABLE       (1 << 0)
#define PAGING_PAGE_READWRITE    (1 << 1)
#define PAGING_PAGE_USER         (1 << 2)

class Paging {
public:
    static void init();
    static u32 getFreeTable();
    static void enable();
};

#endif