#include "../../include/xchat/ServerSockets.hpp"

ServerSockets::ServerSockets(unsigned short len_qeue) : port(4888) {
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (sockaddr *) &address, sizeof(address));
    listen(sockfd, len_qeue);
}

int ServerSockets::wait_accept() {
    clientfd = accept(sockfd, NULL, NULL);

    return clientfd;
}