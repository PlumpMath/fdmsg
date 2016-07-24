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
 * Send data and file descriptors on a socket.
 *
 * # Parameters
 *
 * - sock: An open stream-oriented unix socket
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
