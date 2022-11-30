#include <cstdio>
#include <cstdlib>

#include "../../include/xchat/ServerSockets.hpp"

ServerSockets::ServerSockets(unsigned short len_qeue, int _port) : port(_port) {
    errors = 0;

    std::printf("---------------------------------- logs -----------------------------\n");
    std::printf("[+] init server\n");
    std::printf("[+] init address for socket connection\n");
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    std::printf("[+] creating server on address ");
    PrintSocket(&address);

    std::printf("[+] create socket\n");
    sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
    std::printf("[+] bind socket\n");
    create_bind(sockfd, (sockaddr *) &address, sizeof(address));
    std::printf("[+] server listen port\n");
    change_listen(sockfd, len_qeue);
}

int ServerSockets::wait_accept() {
    std::printf("[+] server wait client\n");
    clientfd = accept(sockfd, NULL, NULL);
    if (clientfd < 0) {
        perror("accept: ");
        errors = errors | ERRSERVACCP;
    }

    std::printf("[+] client connected successful\n");
    return clientfd;
}

ssize_t ServerSockets::server_send(int fd_client, const void *msg, int len, int flags) {
    std::printf("[+] server send message\n");
    ssize_t num = send(fd_client, msg, len, flags);
    if (num == -1) {
        perror("send: ");
        errors = errors | ERRSERVSEND;
    }

    return num;
}

ssize_t ServerSockets::server_recv(int fd_client, void *buf, int len, int flags) {
    std::printf("[+] server wait message...\n");
    ssize_t num = recv(fd_client, buf, len, flags);
    if (num == -1) {
        perror("recv: ");
        errors = errors | ERRSERVRECV;
    }

    std::printf("[+] server get message\n");
    return num;
}

int ServerSockets::close_socket(int __fd) {
    std::printf("[+] close socket\n");
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