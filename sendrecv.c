#include <string.h>
#include "fdmsg.h"

static void prep_msg(
	struct msghdr *msg, struct iovec *iov,
	void *data, size_t data_len,
	void *fdbuf, size_t fds_count) {


	struct cmsghdr *cmsg = fdbuf;

	/* Clear the header: */
	memset(fdbuf, 0, CMSG_SPACE(0));

	/* Set up the file descriptors to be tansferred: */
	msg->msg_control = cmsg;
	msg->msg_controllen = FDMSG_BUFSZ(fds_count);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(int) * fds_count);

	/* Set up the data to be transfered: */
	msg->msg_iovlen = 1;
	msg->msg_iov = iov;
	iov->iov_base = data;
	iov->iov_len = data_len;
}

ssize_t fdmsg_recv(int socket,
	void *data, size_t data_len,
	void *fdbuf, size_t fds_count) {

	struct msghdr msg;
	struct iovec iov;

	prep_msg(&msg, &iov, data, data_len, fdbuf, fds_count);
	return recvmsg(socket, &msg, 0);
}


ssize_t fdmsg_send(int socket,
	void *data, size_t data_len,
	void *fdbuf, size_t fds_count) {

	struct msghdr msg;
	struct iovec iov;

	prep_msg(&msg, &iov, data, data_len, fdbuf, fds_count);
	return sendmsg(socket, &msg, 0);
}
