#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "ServerSockets.hpp"
#include "../Message.hpp"
#include "../Threadpool.hpp"

class Server : public ServerSockets, public Message {
    private:
        uint32_t server_id = 0;
        Threadpool threads = 20;
    
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