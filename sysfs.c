#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// /sys and /proc expose kernel and device state as plain TEXT files. Reading a
// device attribute is just opening a file and parsing its contents — the same
// open/read you already know, no special API.

// Read a single unsigned integer from a sysfs/proc attribute file.
// e.g. "/sys/class/net/eth0/mtu" -> 1500. Returns 0 on success, -1 on error.
int read_uint_attr(const char *path, unsigned long *out) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return -1;                      // attribute missing or no permission
    }
    char line[64];
    if (fgets(line, sizeof(line), f) == NULL) {
        fclose(f);
        return -1;                      // empty file
    }
    fclose(f);

    errno = 0;
    char *end;
    unsigned long v = strtoul(line, &end, 10);   // base-10 parse
    if (end == line || errno != 0) {
        return -1;                      // no digits, or overflow
    }
    *out = v;
    return 0;
}

int main(void) {
    unsigned long nproc;
    // /proc/sys/kernel/... and /sys/... are all readable as text.
    if (read_uint_attr("/proc/sys/kernel/pid_max", &nproc) == 0) {
        printf("pid_max = %lu\n", nproc);
    } else {
        printf("could not read pid_max\n");
    }
    return 0;
}
