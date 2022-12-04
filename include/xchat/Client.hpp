#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "ClientSockets.hpp"
#include "../Message.hpp"


class Client : public ClientSockets, public Message {
    private:
        uint32_t client_id;

    public:
        char name[32];
        void SaveMessageCommand(uint32_t command, uint32_t second_command) override;
        void GetMessageCommand(uint32_t* command, uint32_t* second_command) override;
        
        bool check_error();
        void InputName(char* name, uint32_t new_acc);
        void Connect();
        void SendMessage();
        int RecvMessage();
        void CloseSocket();

        Client(int port);
        Client();
};


#endif          // __CLIENT_HPP__