#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "ServerSockets.hpp"
#include "../Message.hpp"

class Server : public ServerSockets, public Message {
    void SaveMessageCommand(uint32_t command, uint32_t second_command) override;
    void GetMessageCommand(uint32_t* command, uint32_t* second_command) override;


};

#endif          // __SERVER_HPP__