#include <iostream>

#include "../../include/xchat/Server.hpp"

static void ProcessMessage(void* arg);

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

    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    for (; it != itend; ++it) {
        if (FD_ISSET(it->clientfd, &sock_set)) {
            server_recv(it->clientfd, buffer, MSGSIZE, 0);
            SaveMessage(buffer);

            threads.Add(ProcessMessage, buffer);
        }
    }
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

    pthread_mutex_lock(&list_mutex);
    for (; it != itend; ++it)
        if (it->id == id)
            clients.erase(it);
    pthread_mutex_unlock(&list_mutex);
}

Server::Server(int port) : ServerSockets(1, port), Message() {
    count = 1;
}

Server::Server() : ServerSockets(1), Message() {}

void Server::SelectClient() {
    unsigned long fdmax = 0;
    FD_ZERO(&sock_set);

    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    for (; it != itend; ++it) {
        FD_SET(it->clientfd, &sock_set);
        if (it->clientfd > fdmax) fdmax = it->clientfd;
    }

    select(fdmax+1, &sock_set, NULL, NULL, NULL);
}