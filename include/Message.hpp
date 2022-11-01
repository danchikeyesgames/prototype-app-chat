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

#define INDEXSIZE           0
#define INDEXID             4
#define INDEXCONTROLPRIMAR  8
#define INDEXCONTROLSECOND  16
#define INDEXFORMATDATA256  20
#define INDEXFORMATDATA512  22
#define INDEXDATASIZE       24
#define INDEXFORMATNAME     28
#define INDEXNAME           32
#define INDEXNAMESIZE       64
#define INDEXDATA256        256
#define INDEXDATA512        512

#define MSGTOUI32(msg, index, format) ((*((uint32_t *) (&msg[index]))) = format)

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

        void SaveID(uint32_t id);
        void SaveSize(uint32_t size);

        
};

#endif          // __MESSAGE_HPP__