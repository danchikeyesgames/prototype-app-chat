#include <iostream>
#include <cstring>

#include "../../include/xchat/Client.hpp"

int main() {
    Client c;
    char buf[1024] = {0};
    char message[512] = {0};
    char nameto[32] = {0};
    uint32_t command = 0, second = 0;
    char* pointer = NULL;

    c.Connect();

    std::cin >> buf;
    c.InputName(buf, CMMNDCNNCTNW);
    std::cout << "My name: " << buf << "\n";

    while (1) {
        std::cin.getline(message, 512, '\n');

        if (std::strncmp(message, "/msg ", 5) == 0) {
            memcpy(nameto, message + 5, 32);
            c.SaveMessageName(nameto);
            c.SaveMessageCommand(CMMNDSEND, 0);
            c.SaveMessageData(message + 37);
            c.SendMessage();

            c.RecvMessage();
            pointer = (char *) c.LoadMessageData();
            c.GetMessageCommand(&command, &second);

            std::cout << "echo: " << pointer << "\n";
            if (command == CMDMSG) {
                memset(nameto, 0, 32);
                memcpy(nameto, pointer, 32);
                std::cout << nameto << ": " << (char *) (pointer + 32) << "\n";
            }

            memset(nameto, 0, 32);
            memset(message, 0, 512);
        }
        
    }

    c.CloseSocket();
    return 0;
}