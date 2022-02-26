#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>
#include <semaphore.h>

class Condition {
public:
    Condition();
    ~Condition();
    void wait();
    bool signal();
private:
    int waiting;
    sem_t condition_mutex;

    friend class Monitor;
};

class Monitor {
public:
    Monitor();
    ~Monitor();

    void enter();
    void leave();

    void wait(Condition& condition);
    void signal(Condition& condition);
private:
    sem_t mutex;
};


#endif
