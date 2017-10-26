#ifndef TP4_TOWERDEFENSE_SERVER_H
#define TP4_TOWERDEFENSE_SERVER_H


#include <mutex>
#include <vector>
#include "../common/Socket.h"
#include "ThreadedQueue.h"
#include "../common/TextMessage.h"
#include "SocketManager.h"

class Server {

private:
    std::mutex m;
    std::vector<std::reference_wrapper<ThreadedQueue<TextMessage>>> clients;

public:
    Server();
    ~Server();
    void notifyAll(std::string message);
    void addClient(ThreadedQueue<TextMessage> &queue);
};


#endif //TP4_TOWERDEFENSE_SERVER_H
