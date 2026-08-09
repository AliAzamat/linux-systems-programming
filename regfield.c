#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// Read a 32-bit register value at byte `offset` within a device file, WITHOUT
// disturbing any other reader's file position. pread takes the offset directly.
static int read_reg32(int fd, off_t offset, uint32_t *out) {
    uint32_t v;
    ssize_t r = pread(fd, &v, sizeof(v), offset);   // positioned read
    if (r != (ssize_t)sizeof(v)) {
        return -1;                                  // short read or error
    }
    *out = v;
    return 0;
}

// Write a 32-bit value at a specific offset, again without touching the cursor.
static int write_reg32(int fd, off_t offset, uint32_t v) {
    ssize_t w = pwrite(fd, &v, sizeof(v), offset);  // positioned write
    if (w != (ssize_t)sizeof(v)) {
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <device-file>\n", argv[0]);
        return 2;
    }
    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        return 1;
    }

    uint32_t status;
    if (read_reg32(fd, 0x00, &status) == 0) {       // STATUS at offset 0
        printf("status = 0x%08X\n", status);
    }

    close(fd);
    return 0;
}
