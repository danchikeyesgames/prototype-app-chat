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
};

#endif      // __SERVER_SOCKETS_HPP__