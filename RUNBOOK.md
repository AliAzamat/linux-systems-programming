# User-space DPU tooling — runbook

## Build
    make all        # build every tool (warnings are errors)

## The tools
- `catfile <path>`     — dump a file via raw read/write syscalls.
- `regpoke`            — mmap /dev/mem and read/modify a register (needs root).
- `regfield <devfile>` — read a 32-bit register by offset with pread.
- `wait_ready`         — block on poll until stdin is readable (or 2s timeout).
- `service`            — a signal-aware loop that exits cleanly on Ctrl-C.

## Permissions
- `regpoke` opens `/dev/mem` and REQUIRES root: `sudo ./regpoke`.
- If `/dev/mem` access fails with `Operation not permitted`, the kernel was
  built with `CONFIG_STRICT_DEVMEM`. Use the device's UIO node or a kernel
  driver (next project) instead of raw /dev/mem.

## Safety
- Only ONE owner may poke a register block at a time. If a kernel driver is
  bound to the device, do NOT run regpoke against it — they will fight.
- Always shut `service` down with Ctrl-C / SIGTERM so it releases mappings and
  descriptors; never `kill -9` a tool holding a hardware mapping.

## Troubleshooting
- Reads return 0xFFFFFFFF: device unpowered or off the bus.
- `wait_ready` always times out: nothing is writing to the fd; check the source.
- A tool hangs on exit: a blocking syscall isn't honoring the stop flag — verify
  the EINTR retry and the volatile sig_atomic_t flag.
