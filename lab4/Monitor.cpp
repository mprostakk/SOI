#include "Monitor.h"
#include <iostream>

using namespace std;

Monitor::Monitor() {
    sem_init(&mutex, 0, 1);
}

Monitor::~Monitor() {
    sem_destroy(&mutex);
}

void Monitor::enter() {
    sem_wait(&mutex);
}

void Monitor::leave() {
    sem_post(&mutex);
}

void Monitor::wait(Condition& condition) {
    ++condition.waiting;
    leave();
    condition.wait();
}

void Monitor::signal(Condition& condition) {
    condition.signal();

    // if (condition.signal()) {
    //     enter();
    // }
}

Condition::Condition() {
    waiting = 0;
    sem_init(&condition_mutex, 0, 0);
}

Condition::~Condition() {
    sem_destroy(&condition_mutex);
}

bool Condition::signal() {
    if (waiting > 0) {
        --waiting;
        sem_post(&condition_mutex);
        return true;
    }
    return false;
}

void Condition::wait() {
    sem_wait(&condition_mutex);
}
