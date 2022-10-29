#ifndef __SOCKET_IP_HPP__
#define __SOCKET_IP_HPP__

#include <netinet/in.h>

typedef struct sockaddr_in sockaddress;

void PrintSocket(sockaddress addr);

#endif      // __SOCKET_IP_HPP__