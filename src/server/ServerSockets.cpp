#include <cstdio>
#include <cstdlib>
#include "../../include/xchat/ServerSockets.hpp"

ServerSockets::ServerSockets(unsigned short len_qeue, int _port) : port(_port) {
    errors = 0;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
    create_bind(sockfd, (sockaddr *) &address, sizeof(address));
    change_listen(sockfd, len_qeue);
}

int ServerSockets::wait_accept() {
    clientfd = accept(sockfd, NULL, NULL);
    if (clientfd < 0) {
        perror("accept: ");
        errors = errors | ERRSERVACCP;
    }

    return clientfd;
}

ssize_t ServerSockets::server_send(const void *msg, int len, int flags) {
    ssize_t num = send(clientfd, msg, len, flags);
    if (num == -1) {
        perror("send: ");
        errors = errors | ERRSERVSEND;
    }

    return num; 
}

ssize_t ServerSockets::server_recv(void *buf, int len, int flags) {
    ssize_t num = recv(clientfd, buf, len, flags);
    if (num == -1) {
        perror("recv: ");
        errors = errors | ERRSERVRECV;
    }

    return num;
}

int ServerSockets::close_socket(int __fd) {
    int err = close(__fd);
    if (err < 0) {
        perror("close socket: ");
    }

    return err;
}

unsigned int ServerSockets::isSockErr() {
    return errors & ERRSERVSOCK; 
}

unsigned int ServerSockets::isAcceptErr() {
    return errors & ERRSERVACCP;
}

unsigned int ServerSockets::isSendErr() {
    return errors & ERRSERVSEND;
}

unsigned int ServerSockets::isRecvErr() {
    return errors & ERRSERVRECV;
}

/**
 * PRIVATE SEGMENT
*/

int ServerSockets::create_socket(int domain, int type, int protocol) {
    int sckfd = socket(domain, type, protocol);
    if (sckfd == -1) {
        perror("socket: ");
        errors = errors | ERRSERVSOCK;
    }

    return sckfd;
}

int ServerSockets::create_bind(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len) {
    int bnd = bind(fd, addr, len);
    if (bnd < 0) {
        perror("bind: ");
        errors = errors | ERRSERVBIND;
    }

    return bnd;
}

int ServerSockets::change_listen(int fd, int n) {
    int lsn = listen(fd, n);
    if (lsn < 0) {
        perror("listen: ");
        errors = errors | ERRSERVLIST;
    }

    return lsn;
}