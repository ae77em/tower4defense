#include "IntermediateBuffer.h"

#include <iostream>
#include <string>

IntermediateBuffer::IntermediateBuffer() {}

IntermediateBuffer::~IntermediateBuffer() {}

std::string IntermediateBuffer::getNextData() {
    std::string result = "";

    std::unique_lock<std::mutex> lck(m);
    cv.wait(lck, [this] {
        return hasData() || isEnded();
    });

    if (hasData()) {
        std::string value = buffer.front();
        result.assign(value);
        buffer.pop();
    }

    cv.notify_one();
    lck.unlock();

    return result;
}

void IntermediateBuffer::addData(const std::string &s) {
    std::unique_lock<std::mutex> lck(m);

    buffer.push(s);

    cv.notify_one();
    lck.unlock();
}

bool IntermediateBuffer::isProcessingYet() const {
    return !isClientProcessesEnded || !buffer.empty();
}

bool IntermediateBuffer::isEnded() const {
    return isClientProcessesEnded && buffer.empty();
}

bool IntermediateBuffer::hasData() const {
    return !buffer.empty();
}

void IntermediateBuffer::setClientProcessEnded(bool ended) {
    std::unique_lock<std::mutex> lck(m);

    isClientProcessesEnded = ended;

    cv.notify_one();
    lck.unlock();
}
