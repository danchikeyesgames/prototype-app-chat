#include <iostream>

#include "../../include/xchat/Client.hpp"

int main() {
    Client c;
    char buf[1024] = "danchik";
    char* pointer = NULL;

    c.Connect();
    c.InputName(buf, CMMNDCNNCTNW);
    std::cout << "My name: " << buf << "\n";
    c.SendMessage();
    c.RecvMessage();

    pointer = (char *) c.LoadMessageData();

    std::cout << "echo: " << pointer << "\n";
    
    c.CloseSocket();
    return 0;
}