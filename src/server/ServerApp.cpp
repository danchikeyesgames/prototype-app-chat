#include <iostream>

#include "../../include/xchat/Server.hpp"


int main() {
    Server myserver = 4888;
    char buf[1024];
    void* ptr = NULL;
    
    myserver.WaitClient();
    myserver.recvMessage();
    ptr = myserver.LoadMessageData();

    std::cout << "Message: " << (char *) ptr << "\n";
    myserver.SaveMessageData((char *) ptr);
    myserver.SaveMessageCommand(0, 0);
    myserver.SendMessage();

    myserver.CloseSocket();
    return 0;
}