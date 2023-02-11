#include <iostream>
#include <fcntl.h>
#include <cstring>

#include "../../include/xchat/ServerVar.hpp"
#include "../../include/xchat/Server.hpp"

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
    int num = 0;
    int closefd = 0;
    uint32_t id;

    if (FD_ISSET(sockfd, &sock_set)) {
        WaitClient();
    }

    pthread_mutex_lock(&list_mutex);
    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    for (; it != itend; ++it) {
        if (FD_ISSET(it->clientfd, &sock_set)) {
            num = server_recv(it->clientfd, buffer, MSGSIZE, 0);
            
            if (num == 0) {
                closefd = it->clientfd;
                id      = it->id;
            } else {
                SaveMessage(buffer);
                threads.Add(ProcessMessage, buffer);
            }
        }
    }

    if (closefd != 0) {
        close_socket(closefd);
        DeleteClient(id);
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
                break;
            }
        }
    }
    pthread_mutex_unlock(&count_mutex);

    if (flag == 1) return;

    cfd = wait_accept();
    fcntl(cfd, F_SETFD, O_NONBLOCK);

    std::cout << "\033[38;05;58m[+] write id & fd to list: " << cfd << "\033[39m\n";
    node_t new_node;
    new_node.id = id;
    new_node.clientfd = cfd;

    pthread_mutex_lock(&list_mutex);
    clients.push_back(new_node);
    pthread_mutex_unlock(&list_mutex);
    
    ClearMsg();
    SaveID(server_id);
    SaveMessageCommand(CMDSENDID, 0);
    std::cout << "[+] Send id to client: " << id << std::endl;
    SaveMessageData(&id, sizeof(id));
    SendMessage(id);
}

void Server::CloseSocket() {
    close_socket(clientfd);
}

void Server::DeleteClient(uint32_t id) {
    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    for (; it != itend; ++it) {
        if (it->id == id) {
            clients.erase(it);
            break;
        }
    }
    
    it = clients.begin();
    itend = clients.end();

    std::cout << "list: \n";
    for (; it != itend; ++it) {
        std::cout << it->name << ": " << it->id << "\n";
    }

    pthread_mutex_lock(&count_mutex);
    count[id] = 0;
    pthread_mutex_unlock(&count_mutex);
}

Server::Server(int port) : ServerSockets(1, port), Message() {
    capacity = 20;
    pthread_mutex_init(&count_mutex, NULL);
    memset(count, 0, capacity);
}

Server::Server() : ServerSockets(1), Message() {}

void Server::SelectClient() {
    unsigned long fdmax = 0;
    FD_ZERO(&sock_set);

    std::cout << "\033[36m[+] wait message from sockets..........\033[39m\n";
    pthread_mutex_lock(&list_mutex);
    std::list<node_t>::iterator it = clients.begin();
    std::list<node_t>::iterator itend = clients.end();

    FD_SET(sockfd, &sock_set);
    if (sockfd > fdmax) fdmax = sockfd;

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
    int sentfd, rfd;
    bool client_find = false;

    std::cout << "[+] start treature message\n";
    command = GETMSGIND(buffer, INDEXCONTROLPRIMAR);
    secondcommand = GETMSGIND(buffer, INDEXCONTROLSECOND);

    if (command == CMMNDCONNECT) {
        id = GETMSGIND(buffer, INDEXID);

        if (secondcommand == CMMNDCNNCTNW) {
            if (GETMSGIND16(buffer, INDEXFORMATDATA256) > 0) {
                std::cout << "256\n";
                chrt = (char *) &buffer[INDEXDATA256];
            } else if (GETMSGIND16(buffer, INDEXFORMATDATA512) > 0) {
                chrt = (char *) &buffer[INDEXDATA512];
                std::cout << "512\n";
            }
        }

        std::cout << "\033[92m[+] new user: " << chrt << "\033[39m\n";

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
        
        std::cout << "[+] message ready\n";
        send(sentfd, bufto, 1024, 0);

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
                rfd = it->clientfd;
                break;
            }
        }
        pthread_mutex_unlock(&list_mutex);

        std::cout << "[+] message from " << name << ": " << data << " to " << chrt << "\n";

        if (name[0] == '\0') {
            std::cout << "[+] such name not found\n";
            return;
        } else {
            MSGTOUI32(bufto, INDEXID, 0);
            MSGTOUI32(bufto, INDEXCONTROLPRIMAR + 4, CMDMSG);
            
            if (sizedata + 32 <= 256) {
                MSGTOUI16(bufto, INDEXFORMATDATA256, FORMATCHR);
                MSGTOUI32(bufto, INDEXDATASIZE, sizedata + 32);
                std::memcpy(bufto + INDEXDATA256, name, 32);
                std::memcpy(bufto + INDEXDATA256 + 32, data, sizedata);
            } else if (sizedata + 32 > 256 && sizedata + 32 <= 512) {
                MSGTOUI16(bufto, INDEXFORMATDATA512, FORMATCHR);
                MSGTOUI32(bufto, INDEXDATASIZE, sizedata + 32);
                std::memcpy(bufto + INDEXDATA512, name, 32);
                std::memcpy(bufto + INDEXDATA512 + 32, data, sizedata);
            } else if (sizedata + 32 > 512) {
                MSGTOUI16(bufto, INDEXFORMATDATA256, FORMATCHR);
                MSGTOUI16(bufto, INDEXFORMATDATA512, FORMATCHR);
                MSGTOUI32(bufto, INDEXDATASIZE, sizedata + 32);
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
                    std::cout << "\033[01;38;05;222mfd: " << sentfd << "\033[0;39m\n";
                    client_find = true;
                    break;
                }
            }
            pthread_mutex_unlock(&list_mutex);
            
            if (client_find)
                send(sentfd, bufto, 1024, 0);
            else {
                std::cout << "[+] such name as " << chrt << "not found\n";

                MSGTOUI32(bufto, INDEXCONTROLPRIMAR + 4, CMDNTFNU);
                send(rfd, bufto, 1024, 0);
            }
        }
    } else if (command == CMMNDCHONLINE) {
        id = GETMSGIND(buffer, INDEXID);
        sizedata = GETMSGIND(buffer, INDEXDATASIZE);

        MSGTOUI32(bufto, INDEXID, 0);
        MSGTOUI32(bufto, INDEXCONTROLPRIMAR + 4, CMDONLINE);

        if (sizedata > 512 || sizedata <= 256)
            data = (char *) &buffer[256];
        else
            data = (char *) &buffer[512];
        
        char name[32] = {0};
        int ByteOffset = 0;
        char NewLine = '\n';
        char TheEnd = '\0';

        pthread_mutex_lock(&list_mutex);
        std::list<node_t>::iterator it = clients.begin();
        std::list<node_t>::iterator itend = clients.end();

        for (; it != itend; ++it) {
            if (id == it->id) {
                std::memcpy(name, it->name, 32);
                sentfd = it->clientfd;
                client_find = true;
            }

            std::memcpy(bufto + INDEXDATA256 + ByteOffset, it->name, strlen(it->name));
            std::memcpy(bufto + INDEXDATA256 + ByteOffset + strlen(it->name), &NewLine, 1);
            ByteOffset += strlen(it->name) + 1;
        }

        pthread_mutex_unlock(&list_mutex);

        MSGTOUI16(bufto, INDEXFORMATDATA256, FORMATCHR);
        MSGTOUI16(bufto, INDEXFORMATDATA512, FORMATCHR);
        MSGTOUI32(bufto, INDEXDATASIZE, 0);
        std::memcpy(bufto + INDEXDATA256 + ByteOffset, &TheEnd, 1);
            
        std::cout << "-----------------ONLINE--------------------\n";
        std::cout << (char*) (bufto + INDEXDATA256) << "\n";
            
        if (client_find)
            send(sentfd, bufto, 1024, 0);
        else {
            std::cout << "[+] such name as " << chrt << "not found\n";

            MSGTOUI32(bufto, INDEXCONTROLPRIMAR + 4, CMDNTFNU);
            send(rfd, bufto, 1024, 0);
        }
    
    }
}