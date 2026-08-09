#ifndef IO_UTIL_H
#define IO_UTIL_H

#include <stddef.h>
#include <sys/types.h>

// Read EXACTLY n bytes (unless EOF), retrying short/interrupted reads.
// Returns bytes read (< n only at EOF), or -1 on a real error.
ssize_t read_full(int fd, void *buf, size_t n);

// Write ALL n bytes, retrying short/interrupted writes. Returns 0 on success,
// -1 on a real error.
int write_full(int fd, const void *buf, size_t n);

#endif // IO_UTIL_H
