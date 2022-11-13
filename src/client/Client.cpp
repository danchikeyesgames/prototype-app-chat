#include <iostream>

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

void Client::RecvMessage() {
    uint8_t buffer[MSGSIZE];

    client_recv(buffer, MSGSIZE, 0);
    SaveMessage(buffer);
}

void Client::CloseSocket() {
    close_socket(sockfd);
}

void Client::Connect() {
    uint32_t id, command, sec_command, myid;
    socket_connect();
    std::cout << "[+] set connection successful\n";
    void* ptr = NULL;

    RecvMessage();
    GetMessageCommand(&command, &sec_command);
    id = LoadID();

    if (command == CMDSENDID && id == 0) {
        ptr = LoadMessageData();
        myid = ((uint32_t *) ptr)[0];
        std::cout << "[+] Get new id for client: " << myid << "\n";
    }

    client_id = myid;
}

void Client::InputName(char* name, uint32_t new_acc) {
    std::cout << "[+] Send nickname to server: " << name << "\n";
    SaveMessageCommand(CMMNDCONNECT, new_acc);
    SaveMessageData(name);
    SendMessage();
    RecvMessage();
    uint32_t command, second;
    GetMessageCommand(&command, &second);
    if (command == CMDAPPLY) {
        std::cout << "[+] nickname apply\n";
    } else {
        std::cout << "[+] Message ignored\n";
    }
}

Client::Client(int port) : ClientSockets(port), Message() {}

Client::Client() : ClientSockets(), Message() {}