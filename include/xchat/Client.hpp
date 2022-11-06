#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "ClientSockets.hpp"
#include "../Message.hpp"


class Client : public ClientSockets, public Message {
    public:
        void SaveMessageCommand(uint32_t command, uint32_t second_command) override;
        void GetMessageCommand(uint32_t* command, uint32_t* second_command) override;

        void Connect();
        void SendMessage();
        void recvMessage();
        void WaitClient();
        void CloseSocket();

        Client(int port);
        Client();
};


#endif          // __CLIENT_HPP__