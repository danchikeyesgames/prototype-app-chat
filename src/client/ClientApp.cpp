#include <iostream>
#include <cstring>

#include "../../include/xchat/Client.hpp"

static char* next_word(char* msg);

int main() {
    Client c;
    char buf[1024] = {0};
    char message[512] = {0};
    char nameto[32] = {0};
    uint32_t command = 0, second = 0;
    char* pointer = NULL;
    char* one;
    char* two;
    char* three;

    c.Connect();

    std::cout << "Enter nickname: ";
    std::cin >> buf;
    c.InputName(buf, CMMNDCNNCTNW);
    std::cout << "My name: " << buf << "\n";

    while (1) {
        std::cin.getline(message, 512, '\n');

        if (std::strncmp(message, "/msg ", 5) == 0) {
            one = next_word(message);
            two = next_word(one);
            int three = two - one - 1;
            memcpy(nameto, message + 5, three);
            std::cout << nameto << ": " << three << " bytes\n";
            c.SaveMessageName(nameto);
            c.SaveMessageCommand(CMMNDSEND, 0);
            c.SaveMessageData(message + 5 + three + 1);
            c.SendMessage();

            c.RecvMessage();
            pointer = (char *) c.LoadMessageData();
            c.GetMessageCommand(&command, &second);

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

static char* next_word(char* msg) {
    char* newword = strchr(msg, ' ');
    return newword + 1;
}