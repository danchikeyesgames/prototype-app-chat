#include <cstdio>
#include <string>

#include "../include/SocketIp.hpp"

void PrintSocket(const sockaddress* addr) {
    std::printf("%d.%d.%d.%d:%u\033[39m\n", 
            (ntohl(addr->sin_addr.s_addr) & 0xFF000000) >> 24, (ntohl(addr->sin_addr.s_addr) & 0x00FF0000) >> 16, 
            (ntohl(addr->sin_addr.s_addr) & 0x0000FF00) >> 8, ntohl(addr->sin_addr.s_addr) & 0x000000FF, ntohs(addr->sin_port));
}

void InitAddress(sockaddr_in* adr, uint16_t port) {
    uint32_t ip = 0;

    std::printf("\033[92m[+] create address to connection\n");
    adr->sin_family = AF_INET;
    adr->sin_port = htons(port);
    ip |= 0xC0;
    ip = ip << 8;
    ip |= 0xA8;
    ip = ip << 8;
    ip |= 0x00;
    ip = ip << 8;
    ip |= 0xF4;
    adr->sin_addr.s_addr = htonl(ip);

    std::printf("[+] ip: ");
    PrintSocket(adr);
}