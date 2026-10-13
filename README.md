# Linux Systems Programming — Syscalls, MMIO, and Signals

**Small, focused C tools for the user-space side of hardware bring-up: raw-syscall I/O, short-read-safe helpers, positioned register reads with `pread`, memory-mapped register access through `/dev/mem`, `poll`-based waiting, sysfs parsing, and a signal-safe service loop.**

Before a kernel driver exists, bring-up engineers reach device registers from user space. Each tool here isolates one technique that matters for that work, with comments on the details that bite in practice — `volatile` for MMIO, `O_SYNC` mappings, `EINTR` retries, and why a signal handler should only set a flag.

## The tools

| Tool | Technique |
|---|---|
| `catfile <path>` | Reads a file with only `open`/`read`/`write`/`close` — no stdio buffering |
| `io_util.c` | `read_full` / `write_full`: loop over short reads and writes, retrying on `EINTR` |
| `regfield <device-file>` | Reads a 32-bit register at an offset with `pread`, which doesn't move the shared file position (a `pwrite` helper is included) |
| `regpoke` | `mmap`s a page of physical memory from `/dev/mem` with `O_SYNC`, reads a status register and read-modify-writes a control bit through `volatile` pointers (requires root) |
| `wait_ready` | Blocks on `poll` until stdin is readable or 2 seconds pass, retrying on `EINTR` |
| `service` | Long-running loop that exits cleanly on `SIGINT`/`SIGTERM` using a `volatile sig_atomic_t` flag set by `sigaction` |
| `sysfs.c` | Parses an integer attribute from `/proc` or `/sys` (reads `/proc/sys/kernel/pid_max`) with `strtoul` error checks |

`regpoke` targets a placeholder register block at physical address `0x40000000`. **Don't run it on a real machine** unless that address is a device you own; writing to arbitrary physical memory can crash the system. [RUNBOOK.md](RUNBOOK.md) covers permissions (`CONFIG_STRICT_DEVMEM`), single-owner rules for register blocks, and troubleshooting.

## Building

Requires GCC on Linux.

```bash
git clone https://github.com/AliAzamat/linux-systems-programming.git
cd linux-systems-programming
make all CFLAGS="-Wall -Wextra -g -O0 -std=gnu11"
gcc -Wall -Wextra -o sysfs sysfs.c          # not in the Makefile
```

Then, for example:

```bash
./catfile Makefile
./wait_ready              # type something within 2 s, or let it time out
./service                 # Ctrl-C → "signal received; cleaning up and exiting"
./sysfs                   # pid_max = <value>
```

## Status

With the Makefile's own flags (`-std=c11 -Werror`), `make all` builds `catfile` and `regpoke` but fails on `regfield` and `service`: strict C11 hides the POSIX declarations for `pread`, `pwrite`, and `struct sigaction`, and the unused `write_reg32` helper trips `-Werror`. Switching to `-std=gnu11` (or defining `_POSIX_C_SOURCE=200809L`) and dropping `-Werror`, as above, builds all five tools — verified with GCC on Linux.

Other notes:

- `make` with no target builds only `catfile`, because it's the first rule; use `make all`.
- `regfield` links `io_util.c` but doesn't call it yet.
- No tests are included.
