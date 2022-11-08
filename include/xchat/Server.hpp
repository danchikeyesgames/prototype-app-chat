#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <list>

#include "ServerSockets.hpp"
#include "../Message.hpp"
#include "../Threadpool.hpp"

class Server : public ServerSockets, public Message {
    private:
        struct list_node;
        typedef list_node node_t;
        std::list<node_t> clients;
        static uint32_t count;

        uint32_t server_id = 0;
        Threadpool threads = 20;
        pthread_mutex_t list_mutex;

    public:
        void SaveMessageCommand(uint32_t command, uint32_t second_command) override;
        void GetMessageCommand(uint32_t* command, uint32_t* second_command) override;

        void SaveNewClient();
        void SendMessage();
        void recvMessage();
        void WaitClient();
        void CloseSocket();

        Server(int port);
        Server();
};

#endif          // __SERVER_HPP__