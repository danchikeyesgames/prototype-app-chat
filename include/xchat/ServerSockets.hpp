#ifndef __SERVER_SOCKETS_HPP__
#define __SERVER_SOCKETS_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef NULL
#define NULL 0
#endif

typedef struct sockaddr_in sock_address;

class ServerSockets {
    private:
        int sockfd, clientfd;
        sock_address address;
        unsigned short port;

    public:
        ServerSockets(unsigned short len_qeue);
        int     wait_accept();
        ssize_t server_send(const void *msg, int len, int flags);
        ssize_t server_recv(void *buf, int len, int flags);
        int     close_socket(int __fd);

    private:
        int create_socket(int domain, int type, int protocol);
        int create_bind(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len);
        int change_listen(int fd, int n);
};

#endif      // __SERVER_SOCKETS_HPP__