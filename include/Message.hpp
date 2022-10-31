#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <cstdint>
#include <string>

#define MSGSIZE 1024

class Message {
    private:
        unsigned int sizebuf = MSGSIZE;
        uint8_t msg[MSGSIZE];
        uint8_t msg_send[MSGSIZE];

    public:
        virtual void SaveMessageCommand(uint32_t command, uint32_t second_command) = 0;

        void SaveMessageName(char* str);
        void SaveMessageName(std::string& str);
        void SaveMessageName(uint64_t id);

        void SaveMassegeData(char* data);
        void SaveMessageData(std::string& data);
        void SaveMessageData(uint64_t* str);

        
};

#endif          // __MESSAGE_HPP__