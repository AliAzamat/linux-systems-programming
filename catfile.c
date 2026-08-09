#include <fcntl.h>      // open, O_RDONLY
#include <unistd.h>     // read, close, write
#include <stdio.h>
#include <errno.h>
#include <string.h>

// Read a file using ONLY raw system calls — no stdio buffering. This is the
// bare metal of I/O: open() to get a file descriptor, read() into a buffer,
// close() to release it.
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <path>\n", argv[0]);
        return 2;
    }

    int fd = open(argv[1], O_RDONLY);   // syscall: returns a small int, or -1
    if (fd < 0) {
        fprintf(stderr, "open: %s\n", strerror(errno));
        return 1;
    }

    char buf[4096];
    ssize_t n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {   // syscall: bytes read
        write(STDOUT_FILENO, buf, (size_t)n);        // fd 1 is stdout
    }
    if (n < 0) {
        fprintf(stderr, "read: %s\n", strerror(errno));
    }

    close(fd);          // syscall: release the descriptor
    return 0;
}
