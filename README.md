# KoalaOS

KoalaOS is a small educational operating system, originally written to learn how an OS works. It has a BIOS bootloader in NASM assembly and a freestanding C++ kernel for **32-bit x86**. There is no host operating system or C++ standard library inside the guest.

The goal is to make low-level mechanisms readable: switching to protected mode, handling interrupts, setting up paging, and scheduling two user-mode tasks. This is a historical learning project, not a production OS or a general-purpose desktop. QEMU replaces the original Bochs workflow; the guest remains x86 even when your computer is an Apple Silicon Mac.

## What it does

- Boots from a raw 1.44 MiB floppy image using a legacy PC BIOS.
- Switches from 16-bit real mode to 32-bit protected mode.
- Installs a Global Descriptor Table (GDT), a Task State Segment (TSS), and an Interrupt Descriptor Table (IDT).
- Handles timer and PS/2 keyboard interrupts through the programmable interrupt controller (PIC).
- Enables paging, with a kernel identity mapping and a separate user-memory mapping.
- Runs two ring-3 tasks with timer-driven round-robin scheduling.
- Implements `int 0x30` system calls for printing and reading the timer tick count.
- Writes directly to the VGA text buffer and echoes keyboard input using a US layout.

There is **no shell, filesystem, network stack, or program loader**. Typing demonstrates keyboard interrupts; it does not execute commands.

## Quick start on macOS

Install [Xcode Command Line Tools](https://developer.apple.com/xcode/resources/) and [Homebrew](https://brew.sh/) if you do not already have them:

```sh
xcode-select --install
```

Then install the assembler, emulator, and ELF linker:

```sh
brew install nasm qemu lld
make
make run
```

Run these commands from the repository root. Both Intel and Apple Silicon Macs use the same commands. The build uses Apple's `clang++` with an explicit i386 ELF target, not macOS's native Mach-O target. The Makefile locates Homebrew's `ld.lld` automatically; Apple's system `ld` cannot link this kernel.

`make run` builds the image and opens QEMU's graphical window. Expect `=== KoalaOS ===`, `Kernel loaded!`, and initialization messages marked `OK`. After several seconds, both `[userland] Task 1` and `[userland] Task 2` should appear repeatedly; the display scrolls as they continue running. Click inside the guest and type a letter to see keyboard echo. Quit QEMU with **Command-Q** on macOS; **Control-Option-G** releases captured input.

The original code counts 100 timer ticks between task messages but does not program the PIT to 100 Hz. With the BIOS timer rate, messages arrive roughly every 5–6 seconds, not once per second.

## Linux build

On Debian/Ubuntu, install the equivalent tools:

```sh
sudo apt install make clang lld nasm qemu-system-x86
make
make run
```

The build requires Clang with the i386 target and LLD's ELF linker. The Linux package commands are provided as setup guidance; the macOS build and QEMU boot are the verified workflow.

## Build and emulator commands

```sh
make                 # Build boot/bootloader, kern/kernel, and floppyA
make run             # Build, then boot floppyA in QEMU
make clean           # Remove generated image, binaries, objects, and dependencies
make clean && make   # Rebuild after changing compiler flags or toolchains
```

`floppyA` is a raw disk image, not an ISO or an executable for the host. QEMU attaches it as a read-only floppy and emulates an x86 PC with 32 MiB of RAM. Software emulation (`-accel tcg`) is deliberate: an x86 guest cannot use ARM hardware virtualization on Apple Silicon.

You can boot an already-built image directly:

```sh
qemu-system-i386 -machine pc -accel tcg -m 32 -boot order=a \
  -drive file=floppyA,format=raw,if=floppy,readonly=on -no-reboot
```

Additional QEMU options are passed through `QEMU_FLAGS`. For a terminal text display, where your QEMU build supports curses:

```sh
make run QEMU_FLAGS='-display curses'
```

Use **Alt-2** to switch to the QEMU monitor, enter `quit` to exit, or use **Alt-1** to return to the guest display. `-nographic` is not a substitute for the VGA display: KoalaOS does not send output to a serial console.

Tool paths can be overridden, for example:

```sh
make CXX=clang++ LD=/path/to/ld.lld NASM=/path/to/nasm
make run QEMU=/path/to/qemu-system-i386
```

## How the boot works

1. The PC BIOS loads the first 512-byte floppy sector at physical address `0x7C00`. Its final bytes contain the `0xAA55` boot signature.
2. `boot/bootloader.asm` reads the next 50 sectors into memory at `0x1000`. Reads advance across the floppy's 18-sector tracks and two heads; a failed BIOS disk read displays an error and halts.
3. The bootloader installs a temporary GDT, enables protected mode, and jumps to the kernel at `0x1000`.
4. `kern/start.asm` establishes the kernel stack, enables A20 for memory above 1 MiB, clears uninitialized global data (BSS), and calls `kernel_main`.
5. `kern/kernel.cpp` initializes the display, GDT/TSS, IDT, PIC, and paging. It copies the two demo task functions into user memory and creates their contexts **before** enabling interrupts.
6. Timer interrupts enter the assembly stubs, save CPU state, and call the scheduler. Assembly restores the next task's context and returns to ring 3 with `iret`. Tasks call back into the kernel through `int 0x30`.

`kern/linker.ld` places the assembly entry point first at `0x1000` and lays out code, constants, data, and BSS explicitly. It rejects a kernel whose memory footprint exceeds the loader's fixed **50-sector / 25,600-byte budget**, including BSS. Expanding the kernel beyond that requires updating both the loader and linker memory layout.

## Where to start reading

| File(s) | Learning focus |
| --- | --- |
| `boot/bootloader.asm` | BIOS disk I/O, segment addressing, protected-mode transition |
| `kern/start.asm`, `kern/linker.ld` | Bare-metal entry point, stack, A20, and binary layout |
| `kern/kernel.cpp` | Initialization order and the complete boot sequence |
| `kern/GlobalDescriptorTable.*` | Segmentation, privilege levels, and the TSS |
| `kern/InterruptDescriptorTable.*`, `kern/interruptions.asm` | Interrupt gates, saved CPU state, and interrupt returns |
| `kern/InterruptController.*`, `kern/InterruptServicesRoutines.*` | Hardware IRQs, timer ticks, keyboard events, and system calls |
| `kern/Paging.*` | Page directory/table construction and virtual-to-physical mappings |
| `kern/Scheduler.*`, `kern/userland.cpp` | Process contexts, round-robin scheduling, and ring-3 task code |
| `kern/Screen.*`, `kern/Keyboard.*`, `kern/io.h` | VGA output, scan codes, and port I/O |

A useful first experiment is to change the boot banner in `kern/kernel.cpp`, rebuild, and boot again. Next, follow the print system call from `kern/userland.cpp` through `asm_syscall` into `do_syscall`. Finally, trace a timer interrupt through `asm_irq0_clock`, `Scheduler::schedule`, and `asm_resume_process`.

For hardware background, consult the [Intel Software Developer's Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html), especially the system programming volumes, and the [OSDev Wiki](https://wiki.osdev.org/). [QEMU's system emulator documentation](https://www.qemu.org/docs/master/system/index.html) describes emulator options and debugging facilities.

## Debugging and limitations

Start QEMU paused with its GDB stub enabled:

```sh
make run QEMU_FLAGS='-S -s'
```

An x86-capable GDB can connect with `target remote localhost:1234`; use `set architecture i386` and `continue` to start execution. The generated kernel is a flat binary without debugger symbols, so this setup is primarily for inspecting registers, memory, and instructions.

Keep these historical constraints in mind:

- The build intentionally uses `-O0`. Low-level inline assembly and the demo tasks' fixed-size code copies are not designed for arbitrary compiler optimization.
- Each task is copied as 500 bytes of machine code, not loaded from an executable. Enlarging a task, adding external calls, or referring to kernel globals from it requires revisiting this mechanism.
- Memory allocation, exception handling, and process isolation are incomplete. Tasks share a user-memory mapping, and syscall pointers are not validated. Do not treat ring 3 here as a security boundary.
- Clang reports a pre-existing paging allocator warning: its byte-sized bitmap index cannot cover the entire bitmap. The two initial page tables used by this demo do not reach that boundary.
- The bootloader assumes a 1.44 MiB floppy and legacy BIOS, not UEFI. Use the provided QEMU PC configuration rather than attempting a native macOS boot.
- If tools are not found, check your `PATH` and Homebrew installation. If you get Mach-O linker errors, use `ld.lld`, not Apple's `ld`. If you change build flags, clean before rebuilding.
