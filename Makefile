CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -O0 -std=c11

# io_util is a small library the other tools link against.
COMMON = io_util.c

catfile:    catfile.c
	$(CC) $(CFLAGS) -o catfile catfile.c

regpoke:    regpoke.c
	$(CC) $(CFLAGS) -o regpoke regpoke.c

regfield:   regfield.c $(COMMON)
	$(CC) $(CFLAGS) -o regfield regfield.c $(COMMON)

wait_ready: wait_ready.c
	$(CC) $(CFLAGS) -o wait_ready wait_ready.c

service:    service.c
	$(CC) $(CFLAGS) -o service service.c

all: catfile regpoke regfield wait_ready service

clean:
	rm -f catfile regpoke regfield wait_ready service
