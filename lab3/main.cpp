#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <chrono>
#include <thread>

#include "buffer.h"


using namespace std;


sem_t mutex;
sem_t producer_even;
sem_t producer_odd;
sem_t consumer_even;
sem_t consumer_odd;

Buffer buffer;

int i = 2;
int j = 1;

int number_of_producers_even_waiting = 0;
int number_of_producers_odd_waiting = 0;

int number_of_consumers_even_waiting = 0;
int number_of_consumers_odd_waiting = 0;


void p(Buffer& b) {
    cout << b.top() << endl;
    cout << b.number_of_even() << endl;
    cout << b.number_of_odd() << endl;
    cout << endl;
}


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


void random_sleep() {
    int random_offset = rand() % 100;
    std::chrono::milliseconds timespan(100 + random_offset);
    std::this_thread::sleep_for(timespan);
}


void* thread_producer_even(void* argument) {
    while(true) {

        sem_wait(&mutex);
        if (!can_produce_even()) {
            ++number_of_producers_even_waiting;
            sem_post(&mutex);
            sem_wait(&producer_even);
            --number_of_producers_even_waiting;
        }

        buffer.put(i);
        cout << "Even Producer: " << i << endl;
        i += 2;
        i %= 50;

        if (can_produce_odd() && number_of_producers_odd_waiting > 0) {
            sem_post(&producer_odd);
        }
        else if (can_consume_even() && number_of_consumers_even_waiting > 0) {
            sem_post(&consumer_even);
        }
        else if (can_consume_odd() && number_of_consumers_odd_waiting > 0) {
            sem_post(&consumer_odd);
        }
        else {
            sem_post(&mutex);
        }

        random_sleep();
    }
}


void* thread_producer_odd(void* argument) {
    while (true) {
        sem_wait(&mutex);
        if (!can_produce_odd()) {
            ++number_of_producers_odd_waiting;
            sem_post(&mutex);
            sem_wait(&producer_odd);
            --number_of_producers_odd_waiting;
        }

        buffer.put(j);
        cout << "Odd Producer: " << j << endl;
        j += 2;
        j %= 50;

        if (can_produce_even() && number_of_producers_even_waiting > 0) {
            sem_post(&producer_even);
        }
        else if (can_consume_odd() && number_of_consumers_odd_waiting > 0) {
            sem_post(&consumer_odd);
        }
        else if (can_consume_even() && number_of_consumers_even_waiting > 0) {
            sem_post(&consumer_even);
        }
        else {
            sem_post(&mutex);
        }

        random_sleep();
    }
}


void* thread_consumer_even(void* argument) {
    while(true) {

        sem_wait(&mutex);
        if (!can_consume_even()) {
            ++number_of_consumers_even_waiting;
            sem_post(&mutex);
            sem_wait(&consumer_even);
            --number_of_consumers_even_waiting;
        }

        int popped = buffer.pop();
        cout << "Even Consumer: " << popped << endl;

        if (can_consume_odd() && number_of_consumers_odd_waiting > 0) {
            sem_post(&consumer_odd);
        }
        else if (can_produce_even() && number_of_producers_even_waiting > 0) {
            sem_post(&producer_even);
        }
        else if (can_produce_odd() && number_of_producers_odd_waiting > 0) {
            sem_post(&producer_odd);
        }
        else {
            sem_post(&mutex);
        }
 
        random_sleep();
    }
}

void* thread_consumer_odd(void* argument) {
    while (true) {

        sem_wait(&mutex);
        // Przyklad zakleszczenia
        // if (!can_consume_even()) {

        if (!can_consume_odd()) {
            ++number_of_consumers_odd_waiting;
            sem_post(&mutex);
            sem_wait(&consumer_odd);
            --number_of_consumers_odd_waiting;
        }

        int popped = buffer.pop();
        cout << "Odd Consumer: " << popped << endl;

        if (can_consume_even() && number_of_consumers_even_waiting > 0) {
            sem_post(&consumer_even);
        }
        else if (can_produce_odd() && number_of_consumers_odd_waiting > 0) {
            sem_post(&producer_odd);
        }
        else if (can_produce_even() && number_of_consumers_even_waiting > 0) {
            sem_post(&producer_even);
        }
        else {
            sem_post(&mutex);
        }

        random_sleep();
    }
}


void init_semaphores() {
    sem_init(&mutex, 0, 1);

    sem_init(&producer_even, 0, 0);
    sem_init(&producer_odd, 0, 0);
    sem_init(&consumer_even, 0, 0);
    sem_init(&consumer_odd, 0, 0);
}


void destroy_semaphores() {
    sem_destroy(&mutex);

    sem_destroy(&producer_even);
    sem_destroy(&producer_odd);
    sem_destroy(&consumer_even);
    sem_destroy(&consumer_odd);
}

int main() {
    init_semaphores();
    srand(time(NULL));

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

    destroy_semaphores();
    return 0;
}
