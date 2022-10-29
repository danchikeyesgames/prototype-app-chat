#include <cstdio>

#include "../../include/xchat/ClientSockets.hpp"

ClientSockets::ClientSockets(unsigned short port) : port(port) {
    errors = 0;

    std::printf("create address to connection\n");
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    std::printf("ip:%d.%d.%d.%d:%u\n", 
            address.sin_addr.s_addr & 0xF000, address.sin_addr.s_addr & 0x0F00, address.sin_addr.s_addr & 0x00F0, 
            address.sin_addr.s_addr & 0x000F, port);

    sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
}

int ClientSockets::client_send(const void *msg, int len, int flags) {
    ssize_t num = send(sockfd, msg, len, flags);
    if (num == -1) {
        perror("send: ");
    }

    return num; 
}

int ClientSockets::client_recv(void *buf, int len, int flags) {
    ssize_t num = recv(sockfd, buf, len, flags);
    if (num == -1) {
        perror("recv: ");
    }

    return num;
}

int ClientSockets::socket_connect() {
    return connect_to_serv(sockfd, (sockaddr *) &address, sizeof(address));
}

/**
 * PRIVATE SEGMENT
 */

int ClientSockets::create_socket(int domain, int type, int protocol) {
    int sckfd = socket(domain, type, protocol);
    if (sckfd == -1) {
        perror("socket: ");
        errors = errors | ERRCLNTSOCK;
    }

    return sckfd;
}

int ClientSockets::connect_to_serv(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len) {
    int sock = connect(fd, addr, len);
    if (sock == -1) {
        perror("connect: ");
        errors = errors | ERRCLNTCONN;
    }

    return sock;
}
