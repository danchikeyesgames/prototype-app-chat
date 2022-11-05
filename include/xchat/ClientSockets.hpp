#ifndef __CLIENT_SOCKETS_HPP__
#define __CLIENT_SOCKETS_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../SocketIp.hpp"

#ifndef NULL
#define NULL 0
#endif

#define SUCCESSFULL 0x0000
#define ERRCLNTSOCK 0x0001
#define ERRCLNTCONN 0x0002

const int default_port = 4888;

class ClientSockets {
    private:
        sockaddress address;
        unsigned short port;
        unsigned int errors;

    public:
        ClientSockets(unsigned short port = default_port);
        int client_send(const void *msg, int len, int flags);
        int client_recv(void *buf, int len, int flags);
        int socket_connect();
        int close_socket(int __fd);

    private:
        int create_socket(int domain, int type, int protocol);
        int connect_to_serv(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len);

    protected:
        int sockfd;
};

#endif      // __CLIENT_SOCKETS_HPP__