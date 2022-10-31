#include <cstring>

#include "../include/Message.hpp"

void Message::SaveMessageName(char* str) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);

    memcpy(msg_send + (size_t) 32, str, strlen(str));

    msg_send[28] = 1;
}

void Message::SaveMessageName(std::string& str) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);


    memcpy(msg_send + (size_t) 32, str.c_str(), str.size());

    msg_send[29] = 1;
}

void Message::SaveMessageName(uint64_t id) {
    memset(msg_send + (size_t) 32, 0, (size_t) 32);
    memset(msg_send + (size_t) 28, 0, (size_t) 4);


    memcpy(msg_send + (size_t) 32, &id, sizeof(id));

    msg_send[30] = 1;
}