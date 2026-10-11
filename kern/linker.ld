ENTRY(_start)

SECTIONS
{
    . = 0x1000;
    .text : {
        KEEP(*(.text.start))
        *(.text .text.*)
    }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }
    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }

    /* The loader reads 50 sectors at 0x1000, below itself at 0x7C00. */
    ASSERT(__bss_end <= 0x1000 + 50 * 512,
           "Kernel exceeds the bootloader's 50-sector memory budget")
}
