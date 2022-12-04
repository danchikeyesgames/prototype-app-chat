#include <iostream>
#include <cstring>

#include "../../include/xchat/Client.hpp"

static char* next_word(char* msg);
static void* recv_thread(void* arg);

int main() {
    Client c;
    char buf[1024] = {0};
    char message[512] = {0};
    char nameto[32] = {0};
    uint32_t command = 0, second = 0;
    char* pointer = NULL;
    char* command_ch;
    char* name_cl;
    int three;
    pthread_t thread;

    c.Connect();
    if (c.check_error()) {
        std::cout << "\033[91mfailed connection\033[39m\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "\033[35mEnter nickname: ";
    std::cin >> buf;
    c.InputName(buf, CMMNDCNNCTNW);
    std::cout << "\033[35mMy name: " << buf << "\033[39m\n";
    pthread_create(&thread, NULL, recv_thread, &c);
    pthread_detach(thread);

    while (1) {
        std::cout << "\033[33m";
        std::cin.getline(message, 512, '\n');
        std::cout << "\033[39m";

        if (std::strncmp(message, "/msg ", 5) == 0) {
            command_ch = next_word(message);
            name_cl = next_word(command_ch);
            three = name_cl - command_ch - 1;
            memcpy(nameto, message + 5, three);

            c.SaveMessageName(nameto);
            c.SaveMessageCommand(CMMNDSEND, 0);
            c.SaveMessageData(message + 5 + three + 1);
            c.SendMessage();
        }

        c.ClearMsg();
        memset(nameto, '\0', 32);
    }

    c.CloseSocket();
    return 0;
}

static char* next_word(char* msg) {
    char* newword = strchr(msg, ' ');
    return newword + 1;
}

static void* recv_thread(void* arg) {
    Client* client_class = (Client *) arg;
    char nameto[32] = {0};
    uint32_t command = 0, second = 0;
    char* pointer = NULL;
    int i = 0;

    while (1) {
        i = client_class->RecvMessage();

        if (i == 0) {
            std::cout << "[+] connection lost\n";
            break;
        }

        pointer = (char *) client_class->LoadMessageData();
        client_class->GetMessageCommand(&command, &second);

        if (command == CMDMSG) {
            memset(nameto, 0, 32);
            memcpy(nameto, pointer, 32);
            std::cout << "\033[35m" << nameto << ": " << (char *) (pointer + 32) << "\033[33m\n";
        } else if (command = CMDNTFNU) {
            memset(nameto, 0, 32);
            pointer = (char *) client_class->LoadMessageName();
            memcpy(nameto, pointer, 32);
            std::cout << "\033[35muser with such nickname " << (char *) (pointer + 32) << "not found, message skipped\033[33m\n";
        }

        memset(nameto, 0, 32);
    }

    return NULL;
}