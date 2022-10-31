#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <cstdint>
#include <string>

#define MSGSIZE 1024

#define CMMNDCONNECT    0x0
#define CMMNDEXIT       0x1
#define CMMNDSEND       0x2
#define CMMNDLOADMSG    0x3

#define CMMNDCNNCTNW 0x0
#define CMMNDCNNCTCH 0x1


#define FORMATCHR 0x01
#define FORMATSTR 0x02
#define FORMATU64 0x04


#define MSGTOUI32(msg, format) ((*((uint32_t *) (&msg[28]))) = format)

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
        void SaveMessageData(uint64_t* str, size_t size);

        
};

#endif          // __MESSAGE_HPP__