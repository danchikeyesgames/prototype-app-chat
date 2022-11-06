#include <iostream>

#include "../../include/xchat/Server.hpp"

void Server::SaveMessageCommand(uint32_t command, uint32_t second_command) {
    SavePrimaryTwo(command);
    SaveSecondary(second_command);
}
    
void Server::GetMessageCommand(uint32_t* command, uint32_t* second_command) {
    *command = GetPrimary();
    *second_command = GetSecondary();
}

void Server::SendMessage() {
    SaveID(server_id);
    void* msg = GetMessageSend();
    
    server_send(msg, MSGSIZE, 0);
}

void Server::recvMessage() {
    uint8_t buffer[MSGSIZE];

    server_recv(buffer, MSGSIZE, 0);
    SaveMessage(buffer);
}

void Server::WaitClient() {
    uint32_t id = 167;

    wait_accept();

    ClearMsg();
    SaveID(server_id);
    SaveMessageCommand(CMDSENDID, 0);
    std::cout << "[+] Send id to client: " << id << std::endl;
    SaveMessageData(&id, sizeof(id));
    SendMessage();
}

void Server::CloseSocket() {
    close_socket(clientfd);
}

void Server::SaveNewClient() {
    
}

Server::Server(int port) : ServerSockets(1, port), Message() {}

Server::Server() : ServerSockets(1), Message() {}