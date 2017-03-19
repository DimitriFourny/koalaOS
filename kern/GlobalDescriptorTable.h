#ifndef H_GLOBALDESCRIPTORTABLE
#define H_GLOBALDESCRIPTORTABLE

#include "types.h"
#include "Memory.h"

#define GDT_BASE 0
#define GDT_SIZE 0x100
#define GDT_KERNEL_STACK           0x1FFFF0
#define GDT_USERLAND_PHYSICAL_BASE 0x200000
#define GDT_USERLAND_LINEAR_BASE   0x1000000
#define GDT_USERLAND_LINEAR_STACK (GDT_USERLAND_LINEAR_BASE+0x1000)
#define RING3 3

enum {
    GDT_SEGMENT_NULL, 
    GDT_SEGMENT_CODE,
    GDT_SEGMENT_DATA,
    GDT_SEGMENT_STACK,
    GDT_SEGMENT_USER_CODE,
    GDT_SEGMENT_USER_DATA,
    GDT_SEGMENT_USER_STACK,
    GDT_SEGMENT_TSS
};

struct gdt_descriptor {
    u16 lim0_15;    
    u16 base0_15;
    u8  base16_23;
    u8  access;
    u8  lim16_19 : 4;
    u8  flags    : 4;
    u8  base24_31;
} __attribute__ ((packed));

struct gdtr {
    u16 size;
    u32 offset;
} __attribute__ ((packed));

struct tss {
    u16 previous_task, __previous_task_unused;
    u32 esp0;
    u16 ss0, __ss0_unused;
    u32 esp1;
    u16 ss1, __ss1_unused;
    u32 esp2;
    u16 ss2, __ss2_unused;
    u32 cr3;
    u32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    u16 es, __es_unused;
    u16 cs, __cs_unused;
    u16 ss, __ss_unused;
    u16 ds, __ds_unused;
    u16 fs, __fs_unused;
    u16 gs, __gs_unused;
    u16 ldt_selector, __ldt_sel_unused;
    u16 debug_flag, io_map;
} __attribute__ ((packed));

extern struct gdt_descriptor g_gdt_descriptors[GDT_SIZE];
extern struct gdtr g_gdtr;
extern struct tss g_tss_default;


class GlobalDescriptorTable {
public:
    static void init();
private:
    static void initDescriptor(u32 base, u32 limite, u8 access, u8 flags, struct gdt_descriptor* desc);
};

#endif
