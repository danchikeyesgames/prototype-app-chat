#ifndef __SERVER_VAR_HPP__
#define __SERVER_VAR_HPP__

#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <list>

struct list_node {
    uint32_t id;
    char name[32];
    int clientfd;
};

typedef list_node node_t;

std::list<node_t> clients;
pthread_mutex_t list_mutex;

#endif          // __SERVER_VAR_HPP__