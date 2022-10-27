#include "../include/xchat/ClientSockets.hpp"

ClientSockets::ClientSockets() : port(4888) {
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sockfd, (struct sockaddr *) &address, sizeof(address));
}

int ClientSockets::client_send(const void *msg, int len, int flags) {
    send(sockfd, msg, len, flags);
}

int ClientSockets::client_recv(void *buf, int len, int flags) {
    recv(sockfd, buf, len, flags);
}