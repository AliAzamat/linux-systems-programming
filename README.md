# Linux Systems Programming — Syscalls and mmap

An intermediate project that teaches the user-space side of systems programming on Linux. You learn what a system call is and how file descriptors abstract every resource, then use mmap over /dev/mem to map a hardware register block into your address space and poke it like ordinary memory. This is exactly how user-space drivers and bring-up tools touch a DPU's registers before a kernel driver exists.

Built step-by-step with [KhwajaLabs Build](https://khwajalabs.com).

## Stack
- C
- Linux
- Syscalls
- File Descriptors
- mmap
- MMIO
