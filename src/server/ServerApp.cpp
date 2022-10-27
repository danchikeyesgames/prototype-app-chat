#include <iostream>

#include "../include/xchat/ServerSockets.hpp"


int main(void) {
    ServerSockets server = 1;
    char buf[1024];

    server.wait_accept();

    server.server_recv(buf, 1024, NULL);
    server.server_send(buf, 1024, NULL);
    std::cout << buf << std::endl;

    return 0;
}