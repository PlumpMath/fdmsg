#ifndef PASSFD_H
#define PASSFD_H

#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * Convience wrappers for transmitting file descriptors on stream-oriented
 * Unix sockets. See README.md in the source distribution for some examples
 * of basic usage.
 *
 * The following quirks of the underlying socket API bleed through:
 *
 * - You can't *just* send file descriptors; you must always send at least
 *   one byte of data.
 * - The buffer of file descriptors requires a header with some extra
 *   information in it, so you can't just pass an `int *` to
 *   `fdmsg_{send,recv}`. The `FDMSG_*` macros help with constructing so
 *   called fdbufs, which are the header + array of file descriptors.
 *
 *   You don't need to do any special initialization of the fdbuf; just
 *   use `FDMSG_BUFDATA` to access the array. `fdmsg_{send,recv}` will take
 *   care of the header themselves.
 */


/**
 * `FDMSG_BUFSZ(n)` is the size in bytes of an fdbuf with capacity for `n`
 * file descriptors.
 */
#define FDMSG_BUFSZ(count) \
	CMSG_SPACE((count) * sizeof(int))

/**
 * `FDMSG_BUFDATA(buf)` reutrns a pointer to the first element in the array of
 * file descriptors containd by fdbuf `buf`.
 */
#define FDMSG_BUFDATA(buf) \
	((int *)CMSG_DATA((struct cmsghdr *)buf))

/**
 * Send and recieve data and file descriptors on a socket.
 *
 * # Parameters
 *
 * - sock: An open stream-oriented unix socket on which to send
 * - data: A buffer of bytes to write to the socket (for send) or
 *         store data read from the socket (for recv).
 * - data_len: The amount of data to read/write. must fit within
 *             `data`. Additionally, `data_len` must always be at
 *             least 1; Sending only file descriptors is not supported.
 * - fdbuf: An fdbuf which is used like `data`, but for file
 *          descriptors.
 * - fd_count: The number of file descriptors to read or write. Must
 *             fit within `fdbuf`.
 *
 * # Return value
 *
 * - On success, returns the number of bytes sent.
 * - On failure, returns -1 and sets errno.
 */
ssize_t fdmsg_send(int sock,
	void *data, size_t data_len,
	void *fdbuf, size_t fd_count);
ssize_t fdmsg_recv(int sock,
	void *data, size_t data_len,
	void *fdbuf, size_t fd_count);

/**
 * Allocate a socket. This wraps the standard posix `socket()` function
 * and supplies the right arguments for use with this library.
 *
 * Equivalent to `socket(AF_UNIX, SOCK_STREAM, 0)`.
 */
int fdmsg_socket(void);

/**
 * Connect to the unix socket at `path`, and associate it with `sockfd`.
 *
 * This is a thin wrapper around the standard function `connect()`
 */
int fdmsg_connect(int sockfd, const char *path);

/**
 * Bind `sockfd` to the unix socket at `path`.
 *
 * This is a thin wrapper around the standard function `bind()`.
 */
int fdmsg_bind(int sockfd, const char *path);

#ifdef __cplusplus
}
#endif
#endif
