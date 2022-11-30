#include <iostream>

#include "../../include/xchat/Server.hpp"

static void* wait_clients(void* args);

int main() {
    Server myserver = 4888;
    char buf[1024];
    void* ptr = NULL;
    uint32_t one, two;
    pthread_t connect_clients;

    pthread_create(&connect_clients, NULL, wait_clients, &myserver);
    pthread_detach(connect_clients);

    // myserver.WaitClient();
    // myserver.WaitClient();
    
    while (1) {
        std::printf("1111\n");
        myserver.SelectClient();
        myserver.recvMessage();
        std::printf("2222\n");
    }
    
    myserver.CloseSocket();
    return 0;
}

static void* wait_clients(void* args) {
    Server* serv = (Server *) args;

    while (1) {
        serv->WaitClient();
    }
}