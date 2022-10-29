#include <cstdio>

#include "../../include/xchat/ClientSockets.hpp"

ClientSockets::ClientSockets(unsigned short port) : port(port) {
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
}

int ClientSockets::client_send(const void *msg, int len, int flags) {
    return send(sockfd, msg, len, flags);
}

int ClientSockets::client_recv(void *buf, int len, int flags) {
    return recv(sockfd, buf, len, flags);
}

int ClientSockets::connect_to_serv(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len) {
    int sock = connect(fd, addr, len);
    if (sock == -1) {
        perror("connect: ");
    }

    return sock;
}

/**
 * PRIVATE SEGMENT
 */

int ClientSockets::create_socket(int domain, int type, int protocol) {
    int sckfd = socket(domain, type, protocol);
    if (sckfd == -1) {
        perror("socket: ");
    }

    return sckfd;
}