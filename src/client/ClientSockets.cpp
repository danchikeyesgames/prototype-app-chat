#include <cstdio>
#include <cstdlib>

#include "../../include/xchat/ClientSockets.hpp"

ClientSockets::ClientSockets(unsigned short port) : port(port) {
    errors = 0;
    uint32_t ip = 0;

    std::printf("[+] create address to connection\n");
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    ip |= 0xC0;
    ip = ip << 8;
    ip |= 0xA8;
    ip = ip << 8;
    ip |= 0x00;
    ip = ip << 8;
    ip |= 0xF4;
    address.sin_addr.s_addr = htonl(ip);

    std::printf("[+] ip: ");
    PrintSocket(address);

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
    std::printf("[+] wait message...\n");
#endif
    ssize_t num = recv(sockfd, buf, len, flags);
    if (num == -1) {
        perror("recv: ");
    }
#ifdef DEBUG_SERVER
    std::printf("[+] get message\n");
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
        perror("connect: ");
        errors = errors | ERRCLNTCONN;
    }

    return sock;
}
