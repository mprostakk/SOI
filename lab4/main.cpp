#include <vector>
#include <mutex>
#include <iostream>
#include <thread>
#include "Monitor.h"
#include "Buffer.h"

using namespace std;

class SynchronizedBuffer : public Monitor {
public:
    void add_even(int value) {
        enter();

        if (!can_produce_even()) {
            ++number_of_producers_even_waiting;
            wait(producer_even);
            --number_of_producers_even_waiting;
        }

        buffer.put(value);
        cout << "Even Producer: " << value << endl;

        if (can_produce_odd() && number_of_producers_odd_waiting > 0) {
            signal(producer_odd);
        }
        else if (can_consume_even() && number_of_consumers_even_waiting > 0) {
            signal(consumer_even);
        }
        else if (can_consume_odd() && number_of_consumers_odd_waiting > 0) {
            signal(consumer_odd);
        }
        else {
            leave();
        }
    }

    void add_odd(int value) {
        enter();

        if (!can_produce_odd()) {
            ++number_of_producers_odd_waiting;
            wait(producer_odd);
            --number_of_producers_odd_waiting;
        }
        
        buffer.put(value);
        cout << "Odd Producer: " << value << endl;

        if (can_produce_even() && number_of_producers_even_waiting > 0) {
            signal(producer_even);
        }
        else if (can_consume_odd() && number_of_consumers_odd_waiting > 0) {
            signal(consumer_odd);
        }
        else if (can_consume_even() && number_of_consumers_even_waiting > 0) {
            signal(consumer_even);
        }
        else {
            leave();
        }
    }

    int consume_even() {
        enter();

        if (!can_consume_even()) {
            ++number_of_consumers_even_waiting;
            wait(consumer_even);
            --number_of_consumers_even_waiting;
        }

        int popped = buffer.pop();
        cout << "Even Consumer: " << popped << endl;

        if (can_consume_odd() && number_of_consumers_odd_waiting > 0) {
            signal(consumer_odd);
        }
        else if (can_produce_even() && number_of_producers_even_waiting > 0) {
            signal(producer_even);
        }
        else if (can_produce_odd() && number_of_producers_odd_waiting > 0) {
            signal(producer_odd);
        }
        else {
            leave();
        }

        return popped;
    }

    int consume_odd() {
        enter();

        if (!can_consume_odd()) {
            ++number_of_consumers_odd_waiting;
            wait(consumer_odd);
            --number_of_consumers_odd_waiting;
        }

        int popped = buffer.pop();
        cout << "Odd Consumer: " << popped << endl;

        if (can_consume_even() && number_of_consumers_even_waiting > 0) {
            signal(consumer_even);
        }
        else if (can_produce_odd() && number_of_consumers_odd_waiting > 0) {
            signal(producer_odd);
        }
        else if (can_produce_even() && number_of_consumers_even_waiting > 0) {
            signal(producer_even);
        }
        else {
            leave();
        }

        return popped;
    }

private:
    bool can_produce_even() {
        return buffer.number_of_even() < 10;
    }

    bool can_consume_even() {
        if (buffer.empty()) {
            return false;
        }
        if (buffer.top() % 2 == 0 && buffer.number_of_even() >= 3) {
            return true;
        }
        return false;
    }

    bool can_produce_odd() {
        return buffer.number_of_odd() < buffer.number_of_even();
    }

    bool can_consume_odd() {
        if (buffer.empty()) {
            return false;
        }
        if (buffer.top() % 2 == 1) {
            return true;
        }
        return false;
    }

    Condition producer_even;
    Condition producer_odd;
    Condition consumer_even;
    Condition consumer_odd;

    int number_of_producers_even_waiting = 0;
    int number_of_producers_odd_waiting = 0;

    int number_of_consumers_even_waiting = 0;
    int number_of_consumers_odd_waiting = 0;

    Buffer buffer;
};


void random_sleep() {
    int random_offset = rand() % 100;
    std::chrono::milliseconds timespan(100 + random_offset);
    std::this_thread::sleep_for(timespan);
}

SynchronizedBuffer buffer;

void* thread_producer_even(void* argument) {
    int x = 2;
    while(true) {
        buffer.add_even(x);
        x += 2;
        x %= 50;
        random_sleep();
    }
}

void* thread_producer_odd(void* argument) {
    int x = 1;
    while(true) {
        buffer.add_odd(x);
        x += 2;
        x %= 50;
        random_sleep();
    }
}

void* thread_consumer_even(void* argument) {
    while(true) {
        int popped = buffer.consume_even();
        random_sleep();
    }
}

void* thread_consumer_odd(void* argument) {
    while(true) {
        int popped = buffer.consume_odd();
        random_sleep();
    }
}

int main() {

    cout << "p - add even producer" << endl;
    cout << "P - add odd producer" << endl;
    cout << "c - add even consumer" << endl;
    cout << "C - add odd consumer" << endl;

    vector<pthread_t> thread_list;

    char x;
    while(cin >> x) {
        if (x == 'q' || x == 'Q') {
            cout << "Quitting" << endl;
            break;
        }

        pthread_t t;
        if (x == 'p') {
            pthread_create(&t, NULL, &thread_producer_even, NULL);
        }
        else if (x == 'P') {
            pthread_create(&t, NULL, &thread_producer_odd, NULL);
        }
        else if (x == 'c') {
            pthread_create(&t, NULL, &thread_consumer_even, NULL);
        }
        else if (x == 'C') {
            pthread_create(&t, NULL, &thread_consumer_odd, NULL);
        }
        else {
            continue;
        }

        thread_list.push_back(t);
    }

    for (auto thread : thread_list) {
        pthread_join(thread, NULL);
    }

    return 0;
}