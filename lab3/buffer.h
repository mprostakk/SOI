#ifndef FIFO_H
#define FIFO_H

#include <queue>
#include <semaphore.h>

class Buffer {
public:
    void put(int value);
    int pop();
    int top() const;
    int number_of_even() const;
    int number_of_odd() const;
    bool empty() const;
private:
    std::deque<int> _buffer;
};

#endif