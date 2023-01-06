#ifndef __SOCKET_IP_HPP__
#define __SOCKET_IP_HPP__

#include <netinet/in.h>

typedef struct sockaddr_in sockaddress;

void PrintSocket(const sockaddress* addr);
void InitAddress(sockaddr_in* adr, uint16_t port);

#endif      // __SOCKET_IP_HPP__