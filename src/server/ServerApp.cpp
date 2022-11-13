#include <iostream>

#include "../../include/xchat/Server.hpp"


int main() {
    Server myserver = 4888;
    char buf[1024];
    void* ptr = NULL;
    uint32_t one, two;


    myserver.WaitClient();
    myserver.WaitClient();
    
    while (1) {
        myserver.SelectClient();
        myserver.recvMessage();
    }
    
    myserver.CloseSocket();
    return 0;
}