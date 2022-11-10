#include <iostream>

#include "../../include/xchat/Server.hpp"

struct Server::list_node {
    uint32_t id;
    char* name[32];
    int clientfd;
};

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
    uint32_t id = count;
    int cfd;
    ++count;

    cfd = wait_accept();

    ClearMsg();
    SaveID(server_id);
    SaveMessageCommand(CMDSENDID, 0);
    std::cout << "[+] Send id to client: " << id << std::endl;
    SaveMessageData(&id, sizeof(id));
    SendMessage();

    node_t new_node;
    new_node.id = id;
    new_node.clientfd = cfd;

    pthread_mutex_lock(&list_mutex);
    clients.push_back(new_node);
    pthread_mutex_unlock(&list_mutex);
}

void Server::CloseSocket() {
    close_socket(clientfd);
}

void Server::DeleteClient(int id) {
    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    for (; it != itend; ++it)
        if (it->id == id)
            clients.erase(it);
}

Server::Server(int port) : ServerSockets(1, port), Message() {
    count = 1;
}

Server::Server() : ServerSockets(1), Message() {}