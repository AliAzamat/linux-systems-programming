#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define PAGE_SIZE 4096UL

// A DPU's status register block lives at a fixed PHYSICAL address. We map it
// into our process so we can read/write it like ordinary memory.
#define REG_BASE   0x40000000UL   // physical base of the register block
#define STATUS_OFF 0x00            // status register, byte offset within block
#define CONTROL_OFF 0x04           // control register

int main(void) {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);  // O_SYNC: no caching of MMIO
    if (fd < 0) {
        fprintf(stderr, "open /dev/mem (need root): %s\n", strerror(errno));
        return 1;
    }

    // mmap requires a page-aligned physical offset. REG_BASE already is.
    void *map = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                     MAP_SHARED, fd, (off_t)REG_BASE);
    if (map == MAP_FAILED) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    // `volatile`: the value can change outside the program (hardware!), so the
    // compiler must NOT cache it in a register or optimize the access away.
    volatile uint32_t *status  = (volatile uint32_t *)((char *)map + STATUS_OFF);
    volatile uint32_t *control = (volatile uint32_t *)((char *)map + CONTROL_OFF);

    printf("status = 0x%08X\n", *status);          // read a register

    *control = *control | (1u << 0);               // read/modify/write: set bit 0

    munmap(map, PAGE_SIZE);
    close(fd);
    return 0;
}
