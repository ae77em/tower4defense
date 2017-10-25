#ifndef INTERMEDIATE_BUFFER_H
#define INTERMEDIATE_BUFFER_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

class IntermediateBuffer {
private:
    std::mutex m;
    std::condition_variable cv;
    std::queue<std::string> buffer;
    bool isClientProcessesEnded = false;
    
public:
    IntermediateBuffer();
    IntermediateBuffer(const IntermediateBuffer& orig);
    virtual ~IntermediateBuffer();

    /*
     * Return the next registry stored in the buffer.
     */
    std::string getNextData();

    /*
     * Add a new string registry to the buffer.
     */
    void addData(const std::string &s);

    /*
     * Return true if is not empty or have some client
     * consuming the buffer.
     */
    bool isProcessingYet() const;

    /*
     * Return true if has data in the buffer.
     */
    bool hasData() const;

    /*
     * Return true if has no more data and the client processes
     * ended.
     */
    bool isEnded() const;

    /*
     * Set the status of ending of the client processes.
     */
    void setClientProcessEnded(bool ended);
};

#endif /* INTERMEDIATE_BUFFER_H */

