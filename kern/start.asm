[BITS 32]
section .text.start
global _start
extern kernel_main, __bss_start, __bss_end

_start:
    cli
    cld
    mov esp, 0x1FFFF0
    xor ebp, ebp

    ; Enable A20 so the stack, page tables and user memory above 1 MiB work.
    in al, 0x92
    and al, 0xFE
    or al, 2
    out 0x92, al

    ; A flat binary does not contain the uninitialized data section.
    mov edi, __bss_start
    mov ecx, __bss_end
    sub ecx, edi
    xor eax, eax
    rep stosb

    call kernel_main
.halt:
    cli
    hlt
    jmp .halt
