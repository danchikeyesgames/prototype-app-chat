#include <cstring>

#include "../include/Message.hpp"

void Message::SaveMessageName(char* str) {
    memset(msg_send + INDEXNAME, 0, (size_t) 32);
    memset(msg_send + INDEXFORMATNAME, 0, (size_t) 4);

    memcpy(msg_send + INDEXNAME, str, strlen(str));

    MSGTOUI32(msg_send, INDEXFORMATNAME, FORMATCHR);
    MSGTOUI32(msg_send, INDEXNAMESIZE, strlen(str));
}

void Message::SaveMessageName(std::string& str) {
    memset(msg_send + INDEXNAME, 0, (size_t) 32);
    memset(msg_send + INDEXFORMATNAME, 0, (size_t) 4);

    memcpy(msg_send + INDEXNAME, str.c_str(), str.size());

    MSGTOUI32(msg_send, INDEXFORMATNAME, FORMATSTR);
    MSGTOUI32(msg_send, INDEXNAMESIZE, str.size());
}

void Message::SaveMessageName(uint64_t id) {
    memset(msg_send + INDEXNAME, 0, (size_t) 32);
    memset(msg_send + INDEXFORMATNAME, 0, (size_t) 4);

    memcpy(msg_send + INDEXNAME, &id, sizeof(id));

    MSGTOUI32(msg_send, INDEXFORMATNAME, FORMATU64);
    MSGTOUI32(msg_send, INDEXNAMESIZE, sizeof(id));
}

void Message::SaveMassegeData(char* data) {
    memset(msg_send + INDEXDATA256, 0, (size_t) 768);

    if (strlen(data) > 256 && strlen(data) <= 512) {
        memcpy(msg_send + (size_t) 512, data, strlen(data));
        MSGTOUI32(msg_send, INDEXFORMATDATA512, FORMATCHR);
    } else if (strlen(data) <= 256) {
        memcpy(msg_send + (size_t) 256, data, strlen(data));
        MSGTOUI32(msg_send, INDEXFORMATDATA256, FORMATCHR);
    } else {
        memcpy(msg_send + (size_t) 512, data, (size_t) 512);
        memcpy(msg_send + (size_t) 256, data, (size_t) 256);
        MSGTOUI32(msg_send, INDEXFORMATDATA512, FORMATCHR);
        MSGTOUI32(msg_send, INDEXFORMATDATA256, FORMATCHR);
    }

    MSGTOUI32(msg_send, INDEXDATASIZE, strlen(data));
}

void Message::SaveMessageData(std::string& data) {
    memset(msg_send + INDEXDATA256, 0, (size_t) 768);

    if (data.size() > 256 && data.size() <= 512) {
        memcpy(msg_send + (size_t) 512, data.c_str(), data.size());
        MSGTOUI32(msg_send, INDEXFORMATDATA512, FORMATSTR);
    } else if (data.size() <= 256) {
        memcpy(msg_send + (size_t) 256, data.c_str(), data.size());
        MSGTOUI32(msg_send, INDEXFORMATDATA256, FORMATSTR);
    } else {
        memcpy(msg_send + (size_t) 512, data.c_str(), (size_t) 512);
        memcpy(msg_send + (size_t) 256, data.c_str(), (size_t) 256);
        MSGTOUI32(msg_send, INDEXFORMATDATA512, FORMATSTR);
        MSGTOUI32(msg_send, INDEXFORMATDATA256, FORMATSTR);
    }
}

void Message::SaveMessageData(uint64_t* data, size_t size) {
    memset(msg_send + INDEXDATA256, 0, (size_t) 768);

    if (size > 256 && size <= 512) {
        memcpy(msg_send + (size_t) 512, data, size);
        MSGTOUI32(msg_send, INDEXFORMATDATA512, FORMATU64);
    } else if (size <= 256) {
        memcpy(msg_send + (size_t) 256, data, size);
        MSGTOUI32(msg_send, INDEXFORMATDATA256, FORMATU64);
    } else {
        memcpy(msg_send + (size_t) 512, data, (size_t) 512);
        memcpy(msg_send + (size_t) 256, data, (size_t) 256);
        MSGTOUI32(msg_send, INDEXFORMATDATA512, FORMATU64);
        MSGTOUI32(msg_send, INDEXFORMATDATA256, FORMATU64);
    }
}

void Message::SaveID(uint32_t id) {
    MSGTOUI32(msg_send, INDEXID, id);
}

void Message::SaveSize(uint32_t size) {
    MSGTOUI32(msg_send, INDEXSIZE, size);
}

void Message::SaveMessage(void* buffer) {
    memset(msg, 0, MSGSIZE);

    memcpy(msg, buffer, MSGSIZE);
}

void* Message::LoadMessageName() {
    return &msg[INDEXNAME];
}

uint32_t Message::GetNameFormat() {
    return GETMSGIND(msg, INDEXFORMATNAME);
}

void* Message::LoadMassegeData() {
    if (GETMSGIND(msg, INDEXFORMATDATA256) > 0) {
        return &msg[INDEXFORMATDATA256];
    } else if (GETMSGIND(msg, INDEXFORMATDATA512) > 0) {
        return &msg[INDEXFORMATDATA512];
    }
}

uint32_t Message::LoadID() {
    return GETMSGIND(msg, INDEXID);
}

uint32_t Message::LoadSize() {
    return GETMSGIND(msg, INDEXSIZE);
}

uint32_t Message::LoadSizeData() {
    return GETMSGIND(msg, INDEXDATASIZE);
}

void Message::ClearMsg() {
    memset(msg_send, 0, MSGSIZE);
}

/**
**** PROTECTED SEGMENT
*/

void Message::SavePrimary(uint32_t primary) {
    MSGTOUI32(msg_send, INDEXCONTROLPRIMAR, primary);
}

void Message::SavePrimaryTwo(uint32_t primary) {
    MSGTOUI32(msg_send, INDEXCONTROLPRIMAR + 4, primary);
}

void Message::SaveSecondary(uint32_t secondary) {
    MSGTOUI32(msg_send, INDEXCONTROLSECOND, secondary);
}

uint32_t Message::GetPrimary() {
    return msg_send[INDEXCONTROLPRIMAR];
}

uint32_t Message::GetPrimaryTwo() {
    return msg_send[INDEXCONTROLPRIMAR + 4];
}

uint32_t Message::GetSecondary() {
    return msg_send[INDEXCONTROLSECOND];
}