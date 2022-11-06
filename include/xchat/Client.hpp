#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "ClientSockets.hpp"
#include "../Message.hpp"


class Client : public ClientSockets, public Message {
    private:
        uint32_t client_id;
    public:
        void SaveMessageCommand(uint32_t command, uint32_t second_command) override;
        void GetMessageCommand(uint32_t* command, uint32_t* second_command) override;

        void InputName(char* name, uint32_t new_acc);
        void Connect();
        void SendMessage();
        void RecvMessage();
        void WaitClient();
        void CloseSocket();

        Client(int port);
        Client();
};


#endif          // __CLIENT_HPP__