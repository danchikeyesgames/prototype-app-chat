#ifndef __SERVER_SOCKETS_HPP__
#define __SERVER_SOCKETS_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef NULL
#define NULL 0
#endif

class ServerSockets {
    private:
        int sockfd, clientfd;
        struct sockaddr_in address;
        unsigned short port;

    public:
        ServerSockets(unsigned short len_qeue);
        int wait_accept();
        int server_send(const void *msg, int len, int flags);
        int server_recv(void *buf, int len, int flags);
        int close_socket(int __fd);

    private:
        int create_socket(int domain, int type, int protocol);
        int create_bind(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len);
        int change_listen(int fd, int n);
};

#endif      // __SERVER_SOCKETS_HPP__