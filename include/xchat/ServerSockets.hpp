#ifndef __SERVER_SOCKETS_HPP__
#define __SERVER_SOCKETS_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef NULL
#define NULL 0
#endif

#define SUCCESSFULL 0x00
#define ERRSERVSOCK 0x01
#define ERRSERVBIND 0x02
#define ERRSERVLIST 0x03
#define ERRSERVACCP 0x04
#define ERRSERVSEND 0x05
#define ERRSERVRECV 0x06
#define ERRSERVQEUE 0x07

const int default_port = 4888;

typedef struct sockaddr_in sock_address;

class ServerSockets {
    private:
        int sockfd, clientfd;
        sock_address address;
        unsigned short port;
        unsigned int errors;

    public:
        ServerSockets(unsigned short len_qeue, int port = default_port);
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