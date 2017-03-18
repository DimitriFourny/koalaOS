#ifndef H_PAGING
#define H_PAGING

#define PAGING_PAGE_DIRECTORY  0x100000
#define PAGING_PAGE_TABLE      0x101000
#define PAGING_PAGE_ENTRY_SIZE 4096         // 4kb
#define PAGING_CR0_FLAG        0x80000000

#define PAGING_PAGE_ENABLE       (1 << 0)
#define PAGING_PAGE_READWRITE    (1 << 1)
#define PAGING_PAGE_USER         (1 << 2)


class Paging {
public:
    static void init();
    static void enable();
};

#endif