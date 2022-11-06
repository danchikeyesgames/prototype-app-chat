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

void Client::recvMessage() {
    uint8_t buffer[MSGSIZE];

    client_recv(buffer, MSGSIZE, 0);
    SaveMessage(buffer);
}

void Client::CloseSocket() {
    close_socket(sockfd);
}

void Client::Connect() {
    socket_connect();
}

Client::Client(int port) : ClientSockets(port), Message() {}

Client::Client() : ClientSockets(), Message() {}