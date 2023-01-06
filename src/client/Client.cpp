#include <iostream>
#include <cstring>

#include "../../include/xchat/Client.hpp"

void Client::SaveMessageCommand(uint32_t command, uint32_t second_command) {
    SavePrimary(command);
    SaveSecondary(second_command);
}
    
void Client::GetMessageCommand(uint32_t* command, uint32_t* second_command) {
    *command = GetPrimaryTwo();
    *second_command = GetSecondary();
}

void Client::SendMessage() {
    SaveID(client_id);
    void* msg = GetMessageSend();

    client_send(msg, MSGSIZE, 0);
}

int Client::RecvMessage() {
    uint8_t buffer[MSGSIZE];

    int i = client_recv(buffer, MSGSIZE, 0);
    SaveMessage(buffer);

    return i;
}

void Client::CloseSocket() {
    close_socket(sockfd);
}

void Client::Connect() {
    uint32_t id, command, sec_command, myid;
    
    socket_connect();
    if (errors == ERRCLNTCONN) {
        return;
    }

    std::cout << "\033[92m[+] set connection successful\033[39m\n";
    void* ptr = NULL;

    RecvMessage();
    GetMessageCommand(&command, &sec_command);
    id = LoadID();

    if (command == CMDSENDID && id == 0) {
        ptr = LoadMessageData();
        myid = ((uint32_t *) ptr)[0];
        std::cout << "\033[39m[+] Get new id for client: " << myid << "\033[33m\n";
    }

    client_id = myid;
}

void Client::InputName(char* name, uint32_t new_acc) {
    std::cout << "\033[39m[+] Send nickname to server: " << name << "\n";
    SaveMessageCommand(CMMNDCONNECT, new_acc);
    SaveMessageData(name);
    SendMessage();
    RecvMessage();
    uint32_t command, second;
    GetMessageCommand(&command, &second);
    if (command == CMDAPPLY) {
        std::cout << "\033[39m[+] nickname apply\033[33m\n";
    } else {
        std::cout << "[+] Message ignored\n";
    }
}

Client::Client(int port) : ClientSockets(port), Message() {}

Client::Client() : ClientSockets(), Message() {}

bool Client::check_error() {
    return errors ? true : false;
}