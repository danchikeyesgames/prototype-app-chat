#include "../../include/xchat/Server.hpp"

void Server::SaveMessageCommand(uint32_t command, uint32_t second_command) {
    SavePrimary(command);
    SaveSecondary(second_command);
}
    
void Server::GetMessageCommand(uint32_t* command, uint32_t* second_command) {
    *command = GetPrimary();
    *second_command = GetSecondary();
}

void Server::SendMessage() {
    void* msg = GetMessageSend();

    server_send(msg, MSGSIZE, 0);
}

void Server::recvMessage() {
    uint8_t buffer[MSGSIZE];

    server_recv(buffer, MSGSIZE, 0);
    SaveMessage(buffer);
}

void Server::WaitClient() {
    wait_accept();
}

void Server::CloseSocket() {
    close_socket(clientfd);
}

Server::Server(int port) : ServerSockets(1, port), Message() {}

Server::Server() : ServerSockets(1), Message() {}