#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

// A long-running tool must shut down CLEANLY on Ctrl-C / SIGTERM — releasing
// mappings and descriptors so hardware isn't left half-configured.

// Set asynchronously by the signal handler; read by the main loop. Must be
// volatile sig_atomic_t: volatile so the loop re-reads it, sig_atomic_t so the
// write from the handler is indivisible.
static volatile sig_atomic_t stop = 0;

static void on_signal(int signo) {
    (void)signo;
    stop = 1;                 // the ONLY safe thing to do here: set a flag
}

int main(void) {
    struct sigaction sa = {0};
    sa.sa_handler = on_signal;
    sigaction(SIGINT,  &sa, NULL);   // Ctrl-C
    sigaction(SIGTERM, &sa, NULL);   // kill / systemd stop

    printf("service running; press Ctrl-C to stop\n");
    while (!stop) {
        // ... do one unit of work: poll a device, read telemetry, etc. ...
        sleep(1);
    }

    // Reached only after a signal: this is where we release everything.
    printf("\nsignal received; cleaning up and exiting\n");
    // munmap(...); close(fd); etc.
    return 0;
}
