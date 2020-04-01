extern isr_default, isr_clock, isr_keyboard, do_syscall
global asm_int_default, asm_irq0_clock, asm_irq1_keyboard, asm_syscall

%macro SAVE_REGS 0
  pushad
  push ds
  push es
  push fs
  push gs
  push ebx
  mov bx,0x10
  mov ds,bx
  pop ebx
%endmacro

%macro RESTORE_REGS 0
  pop gs
  pop fs
  pop es
  pop ds
  popad
%endmacro

asm_int_default:
	SAVE_REGS
	call isr_default
	mov al, 0x20
	out 0x20, al
	RESTORE_REGS
	iret

asm_irq0_clock:
	SAVE_REGS
  push esp
	call isr_clock
  add esp, 4            ; Clear the stack
	mov al, 0x20
	out 0x20, al
	RESTORE_REGS
	iret

asm_irq1_keyboard:
	SAVE_REGS
	call isr_keyboard
	mov al, 0x20
	out 0x20, al
	RESTORE_REGS
	iret

asm_syscall:
  SAVE_REGS
  sub esp, 8            ; Two parameters
  mov [esp], eax        ; EAX is the syscall number and the first parameter
  mov [esp+4], esp
  add dword[esp+4], 8   ; The second parameter is the context saved with 
                        ; SAVE_REGS
  call do_syscall
  add esp, 8            ; Clear the stack
  RESTORE_REGS
  iret
