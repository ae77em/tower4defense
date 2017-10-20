#include "ThreadedQueue.h"
#include <string>

using namespace std;

ThreadedQueue::ThreadedQueue() : closed(false) {}

bool ThreadedQueue::isEmpty() {
    return (this->buffer.size() == 0);
}

bool ThreadedQueue::isAtEnd() {
    unique_lock<mutex> lck{this->m};

    if (!this->isEmpty()) return false;

    if (!this->closed)
        this->empty.wait(lck, [this]{return !this->isEmpty() || this->closed;});

    return (this->isEmpty() && this->closed);
}

void ThreadedQueue::push(const string &x) {
    unique_lock<mutex> lck{this->m};

    if (this->closed) throw;

    this->buffer.push_back(x);
    this->empty.notify_one();
}

string ThreadedQueue::pop() {
    unique_lock<mutex> lck{this->m};

    // Wait until buffer is not empty or is closed.
    if (this->isEmpty())
        this->empty.wait(lck, [this]{return this->closed || !this->isEmpty();});

    if (this->isEmpty()) throw;

    string res = this->buffer.front();
    this->buffer.pop_front();

    return res;
}

void ThreadedQueue::close() {
    unique_lock<mutex> lck{this->m};
    this->closed = true;
    this->empty.notify_all();
}
