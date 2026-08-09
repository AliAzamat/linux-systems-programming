#include <poll.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// Block until `fd` has data to read OR the timeout elapses. Returns:
//   1 = readable, 0 = timed out, -1 = error. Far better than spinning on read.
int wait_readable(int fd, int timeout_ms) {
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;          // we care about: readable
    pfd.revents = 0;

    for (;;) {
        int r = poll(&pfd, 1, timeout_ms);
        if (r < 0) {
            if (errno == EINTR) continue;   // interrupted: poll again
            return -1;                      // real error
        }
        if (r == 0) {
            return 0;                       // timeout: nothing became ready
        }
        if (pfd.revents & POLLIN) {
            return 1;                       // fd is readable
        }
        return -1;                          // POLLERR/POLLHUP etc.
    }
}

int main(void) {
    // STDIN_FILENO: wait up to 2 seconds for the user to type something.
    int r = wait_readable(STDIN_FILENO, 2000);
    if (r == 1)      printf("stdin is readable\n");
    else if (r == 0) printf("timed out waiting for input\n");
    else             printf("poll error: %s\n", strerror(errno));
    return 0;
}
