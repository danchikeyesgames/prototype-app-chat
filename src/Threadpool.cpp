#include <iostream>
#include <cstring>

#include "../include/Threadpool.hpp"

struct Threadpool::pth_work {
    pthreadpool_work_t  func;
    void*               arg;
    pth_work* next;
};


Threadpool::Threadpool(size_t num) {
    pthread_t threads;

    first_message = NULL;
    last_message = NULL;

    pthread_mutex_init(&work_mutex, NULL);
    pthread_cond_init(&work_cond, NULL);
    pthread_cond_init(&info_cond, NULL);

    thread = num;
    count = 0;
    stop = false;

    std::cout << "[+] Create Threadpool with " << num << " threads\n";

    for (size_t i = 0; i < thread; ++i) {
        pthread_create(&threads, NULL, main_work, (void *) this);
        pthread_detach(threads);
    }

    std::cout << "[+] threadpool create successful\n";
}

Threadpool::~Threadpool() {
    pth_work_t* tmp = NULL;
    pth_work_t* del_worker = NULL;
    
    
    pthread_mutex_lock(&work_mutex);
    tmp = first_message;
    while (tmp != NULL) {
        del_worker = tmp;
        tmp = tmp->next;
        destroy_work(del_worker);
    }

    stop = true;
    pthread_cond_broadcast(&work_cond);
    pthread_mutex_unlock(&work_mutex);

    Wait();

    pthread_mutex_destroy(&work_mutex);
    pthread_cond_destroy(&work_cond);
    pthread_cond_destroy(&info_cond);

}


void Threadpool::Wait() {
    pthread_mutex_lock(&work_mutex);

    while (thread != 0 && count != 0) {
        pthread_cond_wait(&info_cond, &work_mutex);
    }

    pthread_mutex_unlock(&work_mutex);
}

bool Threadpool::Add(pthreadpool_work_t func, void* arg) {
    pth_work_t* worker = create_work(func, arg);

    pthread_mutex_lock(&work_mutex);
    if (worker == NULL)
        return false;

    if (first_message == NULL) {
        first_message = worker;
        last_message = worker;
    } else {
        last_message->next = worker;
        last_message = worker;
    }

    std::cout << "[+] Add work\n";

    pthread_cond_broadcast(&work_cond);
    pthread_mutex_unlock(&work_mutex);

    return true;
}

/**
 * PRIVATE SEGMENT 
 */

Threadpool::pth_work_t* Threadpool::create_work(pthreadpool_work_t func, void* arg) {
    pth_work_t* worker;

    worker = new pth_work_t;
    worker->func = func;
    worker->arg  = malloc(sizeof(char) * 1024);
    std::memcpy(worker->arg, arg, 1024);
    worker->next = NULL;

    return worker;
}

void* main_work(void* arg) {
    Threadpool* pool = (Threadpool *) arg;
    Threadpool::pth_work_t* work_node = NULL;
    bool stop = false;

    while (1) {
        pthread_mutex_lock(&pool->work_mutex);

        stop = pool->stop;
        while (pool->first_message == NULL && !stop) {
            pthread_cond_wait(&pool->work_cond, &pool->work_mutex);
        }

        std::cout << "[+] thread ready to work\n";

        if (stop)
            break;

        work_node = pool->get_worker();
        ++pool->count;
        pthread_mutex_unlock(&pool->work_mutex);

        if (work_node != NULL) {
            work_node->func(work_node->arg);
            pool->destroy_work(work_node);
        }

        pthread_mutex_lock(&pool->work_mutex);
        --pool->count;
        if (!stop && pool->count == 0 && pool->first_message == NULL)
            pthread_cond_signal(&pool->info_cond);
        pthread_mutex_unlock(&pool->work_mutex);

    }

    --pool->thread;
    pthread_cond_signal(&pool->info_cond);
    pthread_mutex_unlock(&pool->work_mutex);

    return NULL;
}

Threadpool::pth_work_t* Threadpool::get_worker() {
    pth_work_t* worker;
    
    if (first_message == NULL) {
        return NULL;
    }

    worker = first_message;
    first_message = first_message->next;

    if (first_message == NULL) {
        last_message = NULL;
    }

    return worker;
}

void Threadpool::destroy_work(pth_work_t* worker) {
    if (worker == NULL)
        return;

    free(worker->arg);
    delete worker;
}

void Threadpool::exec_func(pthreadpool_work_t foo, void* arg) {
    foo(arg);
}