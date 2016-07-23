
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fdmsg.h"

#define SOCKET_PATH "/tmp/test-fdmsg-socket"


char databuf[128];

int server(int sock) {
	int connfd = accept(sock, NULL, NULL);
	if(connfd < 0) {
		perror("accept");
		close(sock);
		return 1;
	}
	void *fdbuf = calloc(1, FDMSG_BUFSZ(3));
	if(!fdbuf) {
		perror("calloc");
		return 1;
	}
	char data = 0;
	if(fdmsg_recv(connfd, &data, 1, fdbuf, 3) < 0) {
		perror("fdmsg_recv");
		return 1;
	}
	int *fds = FDMSG_BUFDATA(fdbuf);
	while(1) {
		ssize_t len = read(fds[0], &databuf, sizeof databuf);
		if(len < 0) {
			perror("read");
			return 1;
		}
		while(len > 0) {
			ssize_t total_written = 0;
			ssize_t written = write(1, &databuf[total_written], len);
			if(written < 0) {
				perror("write");
				return 1;
			}
			total_written += written;
			len -= written;
		}
	}
	return 0;
}

int client(int sock) {
	void *fdbuf = calloc(1, FDMSG_BUFSZ(3));
	if(!fdbuf) {
		perror("calloc");
		return 1;
	}
	char data = 0;
	int *fds = FDMSG_BUFDATA(fdbuf);
	fds[0] = 0;
	fds[1] = 1;
	fds[2] = 2;
	if(fdmsg_send(sock, &data, 1, fdbuf, 3) < 0) {
		perror("fdmsg_send");
		return 1;
	}
	while(1) {
		sleep(1);
	}
	return 0;
}


int main(int argc, char **argv) {
	if(argc < 2) {
		fprintf(stderr, "Usage : ./main [ srv | clnt ]\n");
		return 1;
	}
	int sock = fdmsg_socket();
	if(sock < 0) {
		perror("make socket");
		return 1;
	}
	if(strcmp(argv[1], "srv") == 0) {
		if(fdmsg_bind(sock, SOCKET_PATH) < 0) {
			perror("bind");
			return 1;
		}
		if(listen(sock, SOMAXCONN) < 0) {
			perror("listen");
			return 1;
		}
		return server(sock);
	} else {
		if(fdmsg_connect(sock, SOCKET_PATH) < 0) {
			perror("connect");
			return 1;
		}
		return client(sock);
	}
}
