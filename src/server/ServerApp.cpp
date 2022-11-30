#include <iostream>

#include "../../include/xchat/Server.hpp"

int main() {
    Server myserver = 4888;
    
    while (1) {
        myserver.SelectClient();
        myserver.recvMessage();
    }
    
    myserver.CloseSocket();
    return 0;
}