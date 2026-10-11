[BITS 16]
[ORG 0x0]

start:             ; Loaded by the BIOS at the address 0x7C00
cli
cld

mov ax, 0x07C0     ; Physical address (A:B) = (A * 0x10) + B
mov ds, ax         ; Data Segment initialization
mov es, ax         ; Extra Segment initialization
mov ax, 0x8000    
mov ss, ax         ; Stack Segment initialization
mov sp, 0xFFF0     ; Stack Pointer initialization
sti

mov [bootdrv], dl  ; DL = boot drive unit

mov si, loadingMsg
call showString    ; Kernel loading...

mov dl, [bootdrv]
xor ax, ax
int 0x13           ; Reset floppy
jc diskError

mov ax, 0x100
mov es, ax
xor bx, bx         ; ES:BX = buffer = 0x1000
mov cx, 0x0002     ; Cylinder 0, sector 2 (sector 1 is the bootloader)
xor dh, dh         ; Head 0
mov bp, 50         ; Load 50 sectors, one at a time across track boundaries

readSector:
    mov dl, [bootdrv]
    mov ax, 0x0201
    push bx
    push cx
    push dx
    push bp
    push es
    int 0x13
    pop es
    pop bp
    pop dx
    pop cx
    pop bx
    jc diskError
    add bx, 512
    inc cl
    cmp cl, 19     ; A 1.44 MiB floppy has 18 sectors per track
    jb nextSector
    mov cl, 1
    xor dh, 1      ; Two heads per cylinder
    jnz nextSector
    inc ch
nextSector:
    dec bp
    jnz readSector

mov ax, 0x07C0
mov es, ax

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
diskErrorMsg: db 'Disk read failed', 13, 10, 0

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
diskError:
    mov si, diskErrorMsg
    call showString
    cli
.halt:
    hlt
    jmp .halt


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

