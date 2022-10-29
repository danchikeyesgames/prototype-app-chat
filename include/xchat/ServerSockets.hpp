#ifndef __SERVER_SOCKETS_HPP__
#define __SERVER_SOCKETS_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef NULL
#define NULL 0
#endif

#define SUCCESSFULL 0x0000
#define ERRSERVSOCK 0x0001
#define ERRSERVBIND 0x0002
#define ERRSERVLIST 0x0004
#define ERRSERVACCP 0x0008
#define ERRSERVSEND 0x0010
#define ERRSERVRECV 0x0020
#define ERRSERVQEUE 0x0040

const int default_port = 4888;

typedef struct sockaddr_in sock_address;

class ServerSockets {
    private:
        int sockfd, clientfd;
        sock_address address;
        unsigned short port;
        unsigned int errors;

    public:
        ServerSockets(unsigned short len_qeue, int _port = default_port);
        unsigned int isSockErr();
        unsigned int isAcceptErr();
        unsigned int isSendErr();
        unsigned int isRecvErr();

    public:                             // protected
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