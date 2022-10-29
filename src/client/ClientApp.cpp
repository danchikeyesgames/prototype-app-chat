#include <iostream>

#include "../../include/xchat/ClientSockets.hpp"

int main() {
    ClientSockets client;
    char buf[1024] = "Hello!";
    char buf2[1024];

    client.socket_connect();
    client.client_send(buf, 1024, 0);
    client.client_recv(buf2, 1024, 0);

    std::cout << "OUR MSG: " << buf << "\n";
    std::cout << "RECV MASG: " << buf2 << std::endl;

    return 0;
}