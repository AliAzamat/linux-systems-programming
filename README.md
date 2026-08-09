# Linux Systems Programming — Syscalls and mmap

A project that utilizes the user-space side of systems programming on Linux. Showcases what a system call is and how file descriptors abstract every resource, then uses mmap over /dev/mem to map a hardware register block into the address space and poke it like ordinary memory. This is exactly how user-space drivers and bring-up tools touch a DPU's registers before a kernel driver exists.

## Stack
- C
- Linux
- Syscalls
- File Descriptors
- mmap
- MMIO
