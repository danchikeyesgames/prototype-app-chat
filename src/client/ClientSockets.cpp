#include <cstdio>

#include "../../include/xchat/ClientSockets.hpp"

ClientSockets::ClientSockets(unsigned short port) : port(port) {
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
    connect(sockfd, (struct sockaddr *) &address, sizeof(address));
}

int ClientSockets::client_send(const void *msg, int len, int flags) {
    return send(sockfd, msg, len, flags);
}

int ClientSockets::client_recv(void *buf, int len, int flags) {
    return recv(sockfd, buf, len, flags);
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