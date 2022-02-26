#ifndef BUFFER_H
#define BUFFER_H

#include <queue>
#include "Monitor.h"

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
