#ifndef PASSFD_H
#define PASSFD_H
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/socket.h>

#define FDMSG_BUFSZ(count) \
	CMSG_SPACE((count) * sizeof(int))

#define FDMSG_BUFDATA(buf) \
	((int *)CMSG_DATA((struct cmsghdr *)buf))

ssize_t fdmsg_send(int sock,
	void *data, size_t data_len,
	void *fdbuf, size_t fd_count);

ssize_t fdmsg_recv(int sock,
	void *data, size_t data_len,
	void *fdbuf, size_t fd_count);

int fdmsg_socket(void);
int fdmsg_connect(int sockfd, const char *path);
int fdmsg_bind(int sockfd, const char *path);

#ifdef __cplusplus
}
#endif
#endif
