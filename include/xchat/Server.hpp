#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <list>

#include "ServerSockets.hpp"
#include "../Message.hpp"
#include "../Threadpool.hpp"

struct list_node;
typedef list_node node_t;

std::list<node_t> clients;
pthread_mutex_t list_mutex;

class Server : public ServerSockets, public Message {
    private:

        static int count[20];
        static int capacity;

        uint32_t server_id = 0;
        Threadpool threads = 20;

        pthread_mutex_t count_mutex;

        fd_set sock_set;

    public:
        void SaveMessageCommand(uint32_t command, uint32_t second_command) override;
        void GetMessageCommand(uint32_t* command, uint32_t* second_command) override;

        void SendMessage(uint32_t id);
        void recvMessage();
        void WaitClient();
        void CloseSocket();
        void SelectClient();

        Server(int port);
        Server();

    private:
        void DeleteClient(uint32_t id);
        
};

void ProcessMessage(void* arg);

#endif          // __SERVER_HPP__