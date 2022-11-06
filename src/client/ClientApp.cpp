#include <iostream>

#include "../../include/xchat/Client.hpp"

int main() {
    Client c;
    char buf[1024] = "Hellow!";
    char* pointer = NULL;

    c.Connect();
    c.SaveMessageData(buf);
    c.SaveMessageCommand(0, 0);
    std::cout << "My message: " << buf << "\n";
    c.SendMessage();
    c.RecvMessage();

    pointer = (char *) c.LoadMessageData();

    std::cout << pointer << "\n";
    
    c.CloseSocket();
    return 0;
}