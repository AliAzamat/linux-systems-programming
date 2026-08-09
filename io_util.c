#include <unistd.h>
#include <errno.h>
#include "io_util.h"

ssize_t read_full(int fd, void *buf, size_t n) {
    char *p = buf;
    size_t got = 0;
    while (got < n) {
        ssize_t r = read(fd, p + got, n - got);
        if (r < 0) {
            if (errno == EINTR) continue;   // interrupted by a signal: retry
            return -1;                      // a real error
        }
        if (r == 0) break;                  // EOF: fewer than n bytes available
        got += (size_t)r;
    }
    return (ssize_t)got;
}

int write_full(int fd, const void *buf, size_t n) {
    const char *p = buf;
    size_t sent = 0;
    while (sent < n) {
        ssize_t w = write(fd, p + sent, n - sent);
        if (w < 0) {
            if (errno == EINTR) continue;   // interrupted: retry the rest
            return -1;
        }
        sent += (size_t)w;                  // write may have moved only some
    }
    return 0;
}
