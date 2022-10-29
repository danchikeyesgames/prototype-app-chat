#include <cstdio>

#include "../include/SocketIp.hpp"

void PrintSocket(sockaddress addr) {
    std::printf("%d.%d.%d.%d:%u\n", 
            (ntohl(addr.sin_addr.s_addr) & 0xFF000000) >> 24, (ntohl(addr.sin_addr.s_addr) & 0x00FF0000) >> 16, 
            (ntohl(addr.sin_addr.s_addr) & 0x0000FF00) >> 8, ntohl(addr.sin_addr.s_addr) & 0x000000FF, ntohs(addr.sin_port));
}