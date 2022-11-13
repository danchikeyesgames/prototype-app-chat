#include <iostream>
#include <fcntl.h>
#include <cstring>

#include "../../include/xchat/Server.hpp"

struct list_node {
    uint32_t id;
    char name[32];
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

void Server::SendMessage(uint32_t id) {

    pthread_mutex_lock(&list_mutex);
    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();
    int fd;

    for (; it != itend; ++it) {
        if (it->id == id) {
            fd = it->clientfd;
            break;
        }
    }
    pthread_mutex_unlock(&list_mutex);

    SaveID(server_id);
    void* msg = GetMessageSend();
    
    server_send(fd, msg, MSGSIZE, 0);
}

void Server::recvMessage() {
    uint8_t buffer[MSGSIZE];

    pthread_mutex_lock(&list_mutex);
    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    for (; it != itend; ++it) {
        if (FD_ISSET(it->clientfd, &sock_set)) {
            server_recv(it->clientfd, buffer, MSGSIZE, 0);
            SaveMessage(buffer);

            threads.Add(ProcessMessage, buffer);
        }
    }
    pthread_mutex_unlock(&list_mutex);
}

void Server::WaitClient() {
    int cfd, flag = 0;
    uint32_t id;

    pthread_mutex_lock(&count_mutex);
    if (capacity == 0) {
        flag = 1;
    } else {
        for (int i = 0; i < 20; ++i) {
            if (count[i] == 0) {
                count[i] = 1;
                id = i;
                --capacity;
            }
        }
    }
    pthread_mutex_unlock(&count_mutex);

    if (flag == 1) return;

    cfd = wait_accept();
    fcntl(cfd, F_SETFL, O_NONBLOCK);

    ClearMsg();
    SaveID(server_id);
    SaveMessageCommand(CMDSENDID, 0);
    std::cout << "[+] Send id to client: " << id << std::endl;
    SaveMessageData(&id, sizeof(id));
    SendMessage(cfd);

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

void Server::DeleteClient(uint32_t id) {
    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    pthread_mutex_lock(&list_mutex);
    for (; it != itend; ++it)
        if (it->id == id)
            clients.erase(it);
    pthread_mutex_unlock(&list_mutex);

    pthread_mutex_lock(&count_mutex);
    
    count[id] = 0;
    
    pthread_mutex_unlock(&count_mutex);
}

Server::Server(int port) : ServerSockets(1, port), Message() {
    capacity = 20;
}

Server::Server() : ServerSockets(1), Message() {}

void Server::SelectClient() {
    unsigned long fdmax = 0;
    FD_ZERO(&sock_set);

    pthread_mutex_lock(&list_mutex);
    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    for (; it != itend; ++it) {
        FD_SET(it->clientfd, &sock_set);
        if (it->clientfd > fdmax) fdmax = it->clientfd;
    }
    pthread_mutex_unlock(&list_mutex);

    select(fdmax+1, &sock_set, NULL, NULL, NULL);
}





void ProcessMessage(void* arg) {
    uint8_t* buffer = (uint8_t *) arg;
    uint32_t command, secondcommand, id, sizename, sizedata;
    uint16_t format;
    char* chrt;
    char* data;
    uint8_t bufto[1024] = {0};
    int sentfd;

    command = GETMSGIND(buffer, INDEXCONTROLPRIMAR);

    if (command == CMMNDCONNECT) {
        id = GETMSGIND(buffer, INDEXID);

        if (secondcommand == CMMNDCNNCTCH) {
            if (GETMSGIND16(buffer, INDEXFORMATDATA256) > 0) {
                chrt = (char *) &buffer[INDEXDATA256];
            } else if (GETMSGIND16(buffer, INDEXFORMATDATA512) > 0) {
                chrt = (char *) &buffer[INDEXDATA512];
            }
        }

        pthread_mutex_lock(&list_mutex);
        std::list<node_t>::iterator it = clients.begin();
        std::list<node_t>::iterator itend = clients.end();

        for (; it != itend; ++it) {
            if (id == it->id) {
                std::memcpy(it->name, chrt, 32);
                sentfd = it->clientfd;
                break;
            }
        }
        pthread_mutex_unlock(&list_mutex);

        MSGTOUI32(bufto, INDEXID, 0);
        MSGTOUI32(bufto, INDEXCONTROLPRIMAR + 4, CMDAPPLY);
        send(sentfd, bufto, 1024, NULL);

    } else if (command == CMMNDSEND) {
        id = GETMSGIND(buffer, INDEXID);
        chrt = (char *) &buffer[INDEXNAME];
        sizedata = GETMSGIND(buffer, INDEXDATASIZE);
        sizename = GETMSGIND(buffer, INDEXNAMESIZE);
        if (sizedata > 512 || sizedata <= 256)
            data = (char *) &buffer[256];
        else
            data = (char *) &buffer[512];
        
        char name[32] = {0};

        pthread_mutex_lock(&list_mutex);
        std::list<node_t>::iterator it = clients.begin();
        std::list<node_t>::iterator itend = clients.end();

        for (; it != itend; ++it) {
            if (id == it->id) {
                std::memcpy(name, it->name, 32);
                break;
            }
        }
        pthread_mutex_unlock(&list_mutex);

        if (name[0] == '\0') {
            //
        } else {
            MSGTOUI32(bufto, INDEXID, 0);
            MSGTOUI32(bufto, INDEXCONTROLPRIMAR + 4, CMDMSG);
            
            if (sizedata + 32 <= 256) {
                MSGTOUI16(buffer, INDEXFORMATDATA256, FORMATCHR);
                MSGTOUI32(buffer, INDEXDATASIZE, sizedata + 32);
                std::memcpy(bufto + INDEXDATA256, name, 32);
                std::memcpy(bufto + INDEXDATA256 + 32, data, sizedata);
            } else if (sizedata + 32 > 256 && sizedata + 32 <= 512) {
                MSGTOUI16(buffer, INDEXFORMATDATA512, FORMATCHR);
                MSGTOUI32(buffer, INDEXDATASIZE, sizedata + 32);
                std::memcpy(bufto + INDEXDATA512, name, 32);
                std::memcpy(bufto + INDEXDATA512 + 32, data, sizedata);
            } else if (sizedata + 32 > 512) {
                MSGTOUI16(buffer, INDEXFORMATDATA256, FORMATCHR);
                MSGTOUI16(buffer, INDEXFORMATDATA512, FORMATCHR);
                MSGTOUI32(buffer, INDEXDATASIZE, sizedata + 32);
                std::memcpy(bufto + INDEXDATA256, name, 32);
                std::memcpy(bufto + INDEXDATA256 + 32, data, sizedata);
            }

            std::memcpy(bufto + INDEXNAME, chrt, 32);
            MSGTOUI32(bufto, INDEXNAMESIZE, sizename);

            pthread_mutex_lock(&list_mutex);
            std::list<node_t>::iterator it = clients.begin();
            std::list<node_t>::iterator itend = clients.end();

            for (; it != itend; ++it) {
                if (!std::strncmp(chrt, it->name, 32)) {
                    sentfd = it->clientfd;
                    break;
                }
            }
            pthread_mutex_unlock(&list_mutex);
            
            send(sentfd, bufto, 1024, NULL);
        }
    }
}