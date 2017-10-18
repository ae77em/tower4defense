#ifndef THREAD_H
#define THREAD_H

#include <thread>

class Thread {
public:
    Thread();
    Thread(const Thread&) = delete;
    Thread(Thread&& other);
    Thread& operator=(const Thread&) = delete;
    Thread& operator=(Thread&& other);
    virtual ~Thread();

    virtual void run() = 0;
    void start();
    void join();

private:
    std::thread thread;
};

#endif /* THREAD_H */

