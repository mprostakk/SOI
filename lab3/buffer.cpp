#include "buffer.h"

void Buffer::put(int value) {
    _buffer.push_front(value);
}

int Buffer::top() const {
    int v = _buffer.back();
    return v;
}

int Buffer::pop() {
    int tmp = _buffer.back();
    _buffer.pop_back();
    return tmp;
}

int Buffer::number_of_even() const {
    int sum = 0;
    for (auto el : _buffer) {
        if (el % 2 == 0) {
            ++sum;
        }
    }
    return sum;
}

int Buffer::number_of_odd() const {
    int sum = _buffer.size() - this->number_of_even();
    return sum;
}

bool Buffer::empty() const {
    return _buffer.empty();
}
