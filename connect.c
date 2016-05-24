#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include "fdmsg.h"

static int fdmsg_addr_init(const char *path, struct sockaddr_un *addr) {
	strncpy(addr->sun_path, path, sizeof addr->sun_path);
	addr->sun_path[(sizeof addr->sun_path) - 1] = '\0';
	if(strlen(addr->sun_path) != strlen(path)) return -1;
	addr->sun_family = AF_UNIX;
	return 0;
}

int fdmsg_socket(void) {
	return socket(AF_UNIX, SOCK_STREAM, 0);
}

static int fdmsg_dosock(
		int sock,
		const char *path,
		int (*fn)(int, const struct sockaddr *, socklen_t)) {

	struct sockaddr_un addr;
	if(fdmsg_addr_init(path, &addr) != 0) return -1;
	return fn(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));
}

int fdmsg_connect(int sockfd, const char *path) {
	return fdmsg_dosock(sockfd, path, connect);
}

int fdmsg_bind(int sockfd, const char *path) {
	return fdmsg_dosock(sockfd, path, bind);
}
