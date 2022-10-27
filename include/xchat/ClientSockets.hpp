#ifndef __CLIENT_SOCKETS_HPP__
#define __CLIENT_SOCKETS_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef NULL
#define NULL 0
#endif

class ClientSockets {
    private:
        int sockfd;
        struct sockaddr_in address;
        unsigned short port;

    public:
        ClientSockets();
        int client_send(const void *msg, int len, int flags);
        int client_recv(void *buf, int len, int flags);
};

#endif      // __CLIENT_SOCKETS_HPP__