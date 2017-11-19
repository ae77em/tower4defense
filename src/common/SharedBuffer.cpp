#include "SharedBuffer.h"

#include <iostream>
#include <string>

SharedBuffer::SharedBuffer() {}

SharedBuffer::~SharedBuffer() {}

std::string SharedBuffer::getNextData() {
    std::string result = "";

    std::unique_lock<std::mutex> lck(m);
    cv.wait(lck, [this] {
        return hasData() || isEnded();
    });

    if (hasData()) {
        //std::string value = buffer.front();
        result.assign(buffer.front());
        buffer.pop();
    }

    cv.notify_one();
    lck.unlock();

    return result;
}

void SharedBuffer::addData(const std::string &s) {
    std::unique_lock<std::mutex> lck(m);

    buffer.push(s);

    cv.notify_one();
    lck.unlock();
}

bool SharedBuffer::isProcessingYet() const {
    return !isClientProcessesEnded || !buffer.empty();
}

bool SharedBuffer::isEnded() const {
    return isClientProcessesEnded && buffer.empty();
}

bool SharedBuffer::hasData() const {
    return !buffer.empty();
}

void SharedBuffer::setClientProcessEnded(bool ended) {
    std::unique_lock<std::mutex> lck(m);

    isClientProcessesEnded = ended;

    cv.notify_one();
    lck.unlock();
}
