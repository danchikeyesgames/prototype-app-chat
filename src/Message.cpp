#include <cstring>

#include "../include/Message.hpp"

void Message::SaveMessageName(char* str) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);

    memcpy(msg_send + (size_t) 32, str, strlen(str));

    MSGTOUI32(msg_send, FORMATCHR);
}

void Message::SaveMessageName(std::string& str) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);


    memcpy(msg_send + (size_t) 32, str.c_str(), str.size());

    MSGTOUI32(msg_send, FORMATSTR);
}

void Message::SaveMessageName(uint64_t id) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);


    memcpy(msg_send + (size_t) 32, &id, sizeof(id));

    MSGTOUI32(msg_send, FORMATU64);
}

void Message::SaveMassegeData(char* data) {
    if (strlen(data) > 256 && strlen(data) <= 512)
        memcpy(msg_send + (size_t) 512, data, strlen(data));
    else if (strlen(data) <= 256)
        memcpy(msg_send + (size_t) 256, data, strlen(data));
    else {
        memcpy(msg_send + (size_t) 512, data, (size_t) 512);
        memcpy(msg_send + (size_t) 256, data, (size_t) 256);
    }
}

void Message::SaveMessageData(std::string& data) {
    if (data.size() > 256 && data.size() <= 512)
        memcpy(msg_send + (size_t) 512, data.c_str(), data.size());
    else if (data.size() <= 256)
        memcpy(msg_send + (size_t) 256, data.c_str(), data.size());
    else {
        memcpy(msg_send + (size_t) 512, data.c_str(), (size_t) 512);
        memcpy(msg_send + (size_t) 256, data.c_str(), (size_t) 256);
    }
}

void Message::SaveMessageData(uint64_t* data, size_t size) {
    if (size > 256 && size <= 512)
        memcpy(msg_send + (size_t) 512, data, size);
    else if (size <= 256)
        memcpy(msg_send + (size_t) 256, data, size);
    else {
        memcpy(msg_send + (size_t) 512, data, (size_t) 512);
        memcpy(msg_send + (size_t) 256, data, (size_t) 256);
    }
}