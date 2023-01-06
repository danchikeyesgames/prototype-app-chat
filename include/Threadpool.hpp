#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include <pthread.h>
#include <unistd.h>

void* main_work(void* arg);

typedef void (*pthreadpool_work_t)(void* arg);

class Threadpool {
    friend void* main_work(void* arg);
    
    private:
        struct pth_work;
        typedef struct pth_work pth_work_t;
        pth_work_t* first_message;
        pth_work_t* last_message;
        pthread_mutex_t work_mutex;
        pthread_cond_t  work_cond;
        pthread_cond_t  info_cond;
        size_t  thread;
        size_t  count;
        bool    stop;

    public:
        Threadpool(size_t num = 2);
        bool Add(pthreadpool_work_t func, void* arg);
        void Wait();
        ~Threadpool();

    private:
        pth_work_t* create_work(pthreadpool_work_t func, void* arg);
        pth_work_t* get_worker();
        void        destroy_work(pth_work_t* worker);
        void        exec_func(pthreadpool_work_t foo, void* arg);
};

#endif          // __THREADPOOL_HPP__