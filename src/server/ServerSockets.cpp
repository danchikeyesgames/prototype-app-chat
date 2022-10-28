#include <cstdio>
#include <cstdlib>
#include "../../include/xchat/ServerSockets.hpp"

ServerSockets::ServerSockets(unsigned short len_qeue) : port(4888) {
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (sockaddr *) &address, sizeof(address));
    listen(sockfd, len_qeue);
}

int ServerSockets::wait_accept() {
    clientfd = accept(sockfd, NULL, NULL);

    return clientfd;
}

int ServerSockets::server_send(const void *msg, int len, int flags) {
    return send(clientfd, msg, len, flags);
}

int ServerSockets::server_recv(void *buf, int len, int flags) {
    return recv(clientfd, buf, len, flags);
}

/**
 * PRIVATE SEGMENT
*/

int ServerSockets::create_socket(int domain, int type, int protocol) {
    int sckfd = socket(domain, type, protocol);
    if (sckfd == -1) {
        perror("socket: ");
        exit(EXIT_FAILURE);
    }

    return sckfd;
}