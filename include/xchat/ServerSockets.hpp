#ifndef __SERVER_SOCKETS_HPP__
#define __SERVER_SOCKETS_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
};

#endif      // __SERVER_SOCKETS_HPP__