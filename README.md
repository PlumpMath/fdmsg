Simple library for sending file descriptors to other processes over
sockets.

# Introduction

The Unix socket API provides facilities for sending file descriptors
over domain sockets. This allows for some interesting possibilities, but
like much of the sockets API, the interface to this functionality is...
arcane.

This library provides a wrapper around this feature that makes it
simpler to work with. It doesn't provide the full range of options; you
can only do stream-oriented connections, for example.

## Examples

TODO: verify that these actually work:

```c
/* client.c */
#include <stdio.h>
#include "fdmsg.h"

int main(void) {
    /* create an FD buffer big enough for one fd, and put our
     * stdout in it. The FDMSG_* macros are necessary due to
     * an implementation detail; see the comments in the header
     * file for a full explanation: */
    char fdbuf[FDMSG_BUFSZ(1)];
    int *fds = FDMSG_DATA(&fdbuf[0]);
    char *data = "Have fun with my stdout!";
    fds[0] = 1;

    /* create the socket and connect: */
    int fd = fdmsg_socket();
    if(fd < 0) {
        perror("fdmsg_socket()");
        return 1;
    }
    if(fdmsg_connect(fd, "/path/to/socket") < 0) {
        perror("fdmsg_connect()");
        return 1;
    }

    /* send some bytes and the file descriptor(s): */
    fdmsg_send(fd, data, strlen(data), fdbuf, 1);

    return 0;
}
```

```c
/* server.c */
    char fdbuf[FDMSG_BUFSZ(1)];
    char data[4096];

    /* create the socket and bind: */
    int fd = fdmsg_socket();
    if(fd < 0) {
        perror("fdmsg_socket()");
        return 1;
    }
    if(fdmsg_bind(fd, "/path/to/socket") < 0) {
        perror("fdmsg_bind()");
        return 1;
    }

    /* recieve some bytes and file descriptors(s): */
    ssize_t data_count = fdmsg_recv(fd, data, sizeof data, fdbuf, 1);

    /* send the data down the recieved file descriptor: */
    write(FDMSG_DATA(fdbuf)[0], data, data_count);

    return 0;
```

# License

ISC (simple permissive); see COPYING.
