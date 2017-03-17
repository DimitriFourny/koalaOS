[BITS 16]
[ORG 0x0]

start:             ; Loaded by the BIOS at the address 0x7C00

mov ax, 0x07C0     ; Physical address (A:B) = (A * 0x10) + B
mov ds, ax         ; Data Segment initialization
mov es, ax         ; Extra Segment initialization
mov ax, 0x8000    
mov ss, ax         ; Stack Segment initialization
mov sp, 0xFFF0     ; Stack Pointer initialization

mov [bootdrv], dl  ; DL = boot drive unit

mov si, loadingMsg
call showString    ; Kernel loading...

xor ax, ax
int 0x13           ; Reset floppy

push es
mov ax, 0x100        
mov es, ax
mov bx, 0          ; ES:BX = buffer = 0x1000
mov ah, 2  
mov al, 50         ; 50 sectors
mov ch, 0          ; Cylinder
mov cl, 2          ; Sector 2, this first is used by our boot loader
mov dh, 0          ; Head
mov dl, [bootdrv]  ; The kernel is in our boot drive
int 0x13           ; Read floppy
pop es

mov ax, gdtend    
mov bx, gdt
sub ax, bx            ; Global Descriptor Table size
dec ax
mov word [gdtptr], ax ; Update the GDT size

xor eax, eax      
xor ebx, ebx
mov ax, ds
mov ecx, eax
shl ecx, 4                 ; CX = DS
mov bx, gdt
add ecx, ebx               ; 0x7C00 + gdt relative position
mov dword [gdtptr+2], ecx  ; Update the GDT offset

cli                ; Disable interrupts
lgdt [gdtptr]      ; Load the GDT
mov eax, cr0
or  ax, 1
mov cr0, eax       ; Protection Enable = 1

jmp loadSegments   ; Clear processor internal caches
loadSegments:

mov ax, 0x10       ; Segments registers
mov ds, ax
mov fs, ax
mov gs, ax
mov es, ax
mov ss, ax
mov esp, 0x9F000    

jmp dword 0x8:0x1000 ; Far jump on our kernel


;--------------------------------------------------------------------

bootdrv:  db 0
loadingMsg: db 'Kernel loading...', 13, 10, 0

gdt:
    dq 0
gdt_cs: ; 0x08
    dw 0xFFFF        ; Limit 0:15
    dw 0x0           ; Base 0:15
    db 0x0           ; Base 16:23
    db 10011011b     ; Access bytes = RWX
    db 11001111b     ; Flags + Limit 16:19
    db 0x0           ; Base 24:31
gdt_ds: ; 0x10
    dw 0xFFFF        ; Limit 0:15
    dw 0x0           ; Base 0:15
    db 0x0           ; Base 16:23
    db 10010011b     ; Access bytes = RW
    db 11001111b     ; Flags + Limit 16:19
    db 0x0           ; Base 24:31
gdtend:
gdtptr:
    dw 0  ; Size
    dd 0  ; Offset

;--------------------------------------------------------------------

showString:
    push ax
    push bx
    xor bx, bx            ; BL = Color / BH = Page number
.showString_begin:
    lodsb                 ; DS:SI => AL
    cmp al, 0             ; AL = Character
    je .showString_end
    mov ah, 0x0E          ; Display char
    int 0x10
    jmp .showString_begin
.showString_end:
    pop bx
    pop ax
    ret

; NOP until 510 bytes
times 510-($-$$) db 144
dw 0xAA55

