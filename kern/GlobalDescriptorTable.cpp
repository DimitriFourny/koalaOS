#define __GDT__
#include "GlobalDescriptorTable.h"

struct gdt_descriptor g_gdt_descriptors[GDT_SIZE];
struct gdtr g_gdtr;
struct tss g_tss_default;

void GlobalDescriptorTable::initDescriptor(u32 base, u32 limite, u8 access, u8 flags, struct gdt_descriptor* desc) {
        desc->lim0_15    = (limite & 0xffff);
        desc->base0_15   = (base & 0xffff);
        desc->base16_23  = (base & 0xff0000) >> 16;
        desc->access     = access;
        desc->lim16_19   = (limite & 0xf0000) >> 16;
        desc->flags      = (flags & 0xf);
        desc->base24_31  = (base & 0xff000000) >> 24;
}

void GlobalDescriptorTable::init() {
    // TSS values
    g_tss_default.debug_flag = 0x00;
    g_tss_default.io_map     = 0x00;
    g_tss_default.esp0       = GDT_USERLAND_STACK;
    g_tss_default.ss0        = GDT_SEGMENT_STACK * sizeof(gdt_descriptor);

    // GDT Descriptors
    Memory::fill(reinterpret_cast<char*>(g_gdt_descriptors), 0, sizeof(g_gdt_descriptors));
    initDescriptor(0x0,                  0x0,                   0x0,  0x0,  &g_gdt_descriptors[GDT_SEGMENT_NULL]);
    initDescriptor(0x0,                  0xFFFFF,               0x9B, 0x0D, &g_gdt_descriptors[GDT_SEGMENT_CODE]);       // code
    initDescriptor(0x0,                  0xFFFFF,               0x93, 0x0D, &g_gdt_descriptors[GDT_SEGMENT_DATA]);       // data
    initDescriptor(0x0,                  0x0,                   0x97, 0x0D, &g_gdt_descriptors[GDT_SEGMENT_STACK]);      // stack
    initDescriptor(0x0,                  0xFFFFF,               0xFB, 0x0D, &g_gdt_descriptors[GDT_SEGMENT_USER_CODE]);  // user code
    initDescriptor(0x0,                  0xFFFFF,               0xF3, 0x0D, &g_gdt_descriptors[GDT_SEGMENT_USER_DATA]);  // user data
    initDescriptor(0x0,                  0x0,                   0xF7, 0x0D, &g_gdt_descriptors[GDT_SEGMENT_USER_STACK]); // user stack
    initDescriptor((u32) &g_tss_default, sizeof(g_tss_default), 0x89, 0x04, &g_gdt_descriptors[GDT_SEGMENT_TSS]);        // Task State Segment

    g_gdtr.size   = (GDT_SIZE * sizeof(gdt_descriptor))-1;
    g_gdtr.offset = GDT_BASE;
    Memory::copy(reinterpret_cast<char*>(g_gdtr.offset), reinterpret_cast<char*>(g_gdt_descriptors), g_gdtr.size+1);

    // Load GDT
    asm("lgdtl (g_gdtr)");
    asm("movw %0, %%ax     \n \
         movw %%ax, %%ds   \n \
         movw %%ax, %%es   \n \
         movw %%ax, %%fs   \n \
         movw %%ax, %%gs   \n \
         ljmp %1, $next    \n \
         next:             \n" 
         : : "i"(GDT_SEGMENT_DATA * sizeof(gdt_descriptor)), "i"(GDT_SEGMENT_CODE * sizeof(gdt_descriptor)));

    // Load TSS
    asm("movw %0, %%ax    \n \
         ltr %%ax"
         : : "i"(GDT_SEGMENT_TSS * sizeof(gdt_descriptor)));
}