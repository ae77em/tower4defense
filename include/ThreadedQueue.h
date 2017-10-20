#include <list>
#include <mutex>
#include <condition_variable>

#ifndef THREADED_QUEUE_H
#define THREADED_QUEUE_H

/* This buffer is first-in-first-out (FIFO), i.e. the elements will pop out
 * from it in the order they were pushed in. */
template<typename T>
class ThreadedQueue {
    bool closed;
    std::list<T> buffer;
    std::mutex m;
    std::condition_variable empty;

public:
    ThreadedQueue();

    void push(const T &x);
    void close();

    T pop();
    bool isAtEnd();

private:
    bool isEmpty();
};

using namespace std;

template<typename T>
ThreadedQueue<T>::ThreadedQueue() : closed(false) {}

template<typename T>
bool ThreadedQueue<T>::isEmpty() {
    return (this->buffer.size() == 0);
}

template<typename T>
bool ThreadedQueue<T>::isAtEnd() {
    unique_lock<mutex> lck{this->m};

    if (!this->isEmpty()) return false;

    if (!this->closed)
        this->empty.wait(lck, [this]{return !this->isEmpty() || this->closed;});

    return (this->isEmpty() && this->closed);
}

template<typename T>
void ThreadedQueue<T>::push(const T &x) {
    unique_lock<mutex> lck{this->m};

    if (this->closed) throw;

    this->buffer.push_back(x);
    this->empty.notify_one();
}

template<typename T>
T ThreadedQueue<T>::pop() {
    unique_lock<mutex> lck{this->m};

    // Wait until buffer is not empty or is closed.
    if (this->isEmpty())
        this->empty.wait(lck, [this]{return this->closed || !this->isEmpty();});

    if (this->isEmpty()) throw;

    T res = this->buffer.front();
    this->buffer.pop_front();

    return res;
}

template<typename T>
void ThreadedQueue<T>::close() {
    unique_lock<mutex> lck{this->m};
    this->closed = true;
    this->empty.notify_all();
}

#endif
