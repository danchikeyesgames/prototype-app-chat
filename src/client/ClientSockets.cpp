#include <cstdio>
#include <cstdlib>

#include "../../include/xchat/ClientSockets.hpp"

#define DEBUG_SERVER

ClientSockets::ClientSockets(unsigned short port) : port(port) {
    errors = 0;
    std::printf("---------------------------------- logs -----------------------------\n");
    InitAddress(&address, port);

    std::printf("[+] create socket\n");
    sockfd = create_socket(AF_INET, SOCK_STREAM, 0);
}

int ClientSockets::client_send(const void *msg, int len, int flags) {
#ifdef DEBUG_SERVER
    std::printf("[+] send message\n");
#endif
    ssize_t num = send(sockfd, msg, len, flags);
    if (num == -1) {
        perror("send: ");
    }

    return num; 
}

int ClientSockets::client_recv(void *buf, int len, int flags) {
#ifdef DEBUG_SERVER
    std::printf("\033[39m[+] wait message...\033[33m\n");
#endif
    ssize_t num = recv(sockfd, buf, len, flags);
    if (num == -1) {
        perror("recv: ");
    }
#ifdef DEBUG_SERVER
    std::printf("\033[39m[+] get message\033[33m\n");
#endif
    return num;
}

int ClientSockets::socket_connect() {
    std::printf("[+] connect to server\n");
    return connect_to_serv(sockfd, (sockaddr *) &address, sizeof(address));
}

int ClientSockets::close_socket(int __fd) {
    std::printf("[+] close socket\n");
    int err = close(__fd);
    if (err < 0) {
        perror("close socket: ");
    }

    return err;
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
        perror("\033[91m[+] connect");
        errors = errors | ERRCLNTCONN;
        std::printf("\033[39m");
    }

    return sock;
}
