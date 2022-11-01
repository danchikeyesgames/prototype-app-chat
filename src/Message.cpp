#include <cstring>

#include "../include/Message.hpp"

void Message::SaveMessageName(char* str) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);

    memcpy(msg_send + (size_t) 32, str, strlen(str));

    MSGTOUI32(msg_send, 28, FORMATCHR);
    MSGTOUI32(msg_send, 64, strlen(str));
}

void Message::SaveMessageName(std::string& str) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);

    memcpy(msg_send + (size_t) 32, str.c_str(), str.size());

    MSGTOUI32(msg_send, 28, FORMATSTR);
    MSGTOUI32(msg_send, 64, str.size());
}

void Message::SaveMessageName(uint64_t id) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);

    memcpy(msg_send + (size_t) 32, &id, sizeof(id));

    MSGTOUI32(msg_send, 28, FORMATU64);
    MSGTOUI32(msg_send, 64, sizeof(id));
}

void Message::SaveMassegeData(char* data) {
    memset(msg_send + (size_t) 256, 0, (size_t) 768);

    if (strlen(data) > 256 && strlen(data) <= 512) {
        memcpy(msg_send + (size_t) 512, data, strlen(data));
        MSGTOUI32(msg_send, 22, FORMATCHR);
    } else if (strlen(data) <= 256) {
        memcpy(msg_send + (size_t) 256, data, strlen(data));
        MSGTOUI32(msg_send, 20, FORMATCHR);
    } else {
        memcpy(msg_send + (size_t) 512, data, (size_t) 512);
        memcpy(msg_send + (size_t) 256, data, (size_t) 256);
        MSGTOUI32(msg_send, 22, FORMATCHR);
        MSGTOUI32(msg_send, 20, FORMATCHR);
    }

    MSGTOUI32(msg_send, 24, strlen(data));
}

void Message::SaveMessageData(std::string& data) {
    memset(msg_send + (size_t) 256, 0, (size_t) 768);

    if (data.size() > 256 && data.size() <= 512) {
        memcpy(msg_send + (size_t) 512, data.c_str(), data.size());
        MSGTOUI32(msg_send, 22, FORMATSTR);
    } else if (data.size() <= 256) {
        memcpy(msg_send + (size_t) 256, data.c_str(), data.size());
        MSGTOUI32(msg_send, 20, FORMATSTR);
    } else {
        memcpy(msg_send + (size_t) 512, data.c_str(), (size_t) 512);
        memcpy(msg_send + (size_t) 256, data.c_str(), (size_t) 256);
        MSGTOUI32(msg_send, 22, FORMATSTR);
        MSGTOUI32(msg_send, 20, FORMATSTR);
    }
}

void Message::SaveMessageData(uint64_t* data, size_t size) {
    memset(msg_send + (size_t) 256, 0, (size_t) 768);

    if (size > 256 && size <= 512) {
        memcpy(msg_send + (size_t) 512, data, size);
        MSGTOUI32(msg_send, 22, FORMATU64);
    } else if (size <= 256) {
        memcpy(msg_send + (size_t) 256, data, size);
        MSGTOUI32(msg_send, 20, FORMATU64);
    } else {
        memcpy(msg_send + (size_t) 512, data, (size_t) 512);
        memcpy(msg_send + (size_t) 256, data, (size_t) 256);
        MSGTOUI32(msg_send, 22, FORMATU64);
        MSGTOUI32(msg_send, 20, FORMATU64);
    }
}