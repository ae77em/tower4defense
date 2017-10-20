#include <list>
#include <mutex>
#include <string>
#include <condition_variable>

#ifndef THREADED_QUEUE_H
#define THREADED_QUEUE_H

/* This buffer is first-in-first-out (FIFO), i.e. the elements will pop out
 * from it in the order they were pushed in. */
class ThreadedQueue {
    const unsigned int maxsize;
    bool closed;
    std::list<std::string> buffer;
    std::mutex m;
    std::condition_variable full;
    std::condition_variable empty;

public:
    ThreadedQueue();
    ThreadedQueue(unsigned int maxsize);

    void push(const std::string &x);
    void close();

    std::string pop();
    bool isAtEnd();

private:
    bool isEmpty();
    bool isFull();
};

#endif
