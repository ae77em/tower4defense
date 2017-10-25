#ifndef TP4_TOWERDEFENSE_LISTENER_H
#define TP4_TOWERDEFENSE_LISTENER_H


#include <Thread.h>
#include <Socket.h>
#include "IntermediateBuffer.h"

class ClientListener : public Thread {
private:
    IntermediateBuffer &buffer;
    Socket server;
    std::string host;
    uint16_t port;

public:
    ClientListener(std::string h, uint16_t p, IntermediateBuffer &b);

    virtual ~ClientListener();

    void run();

};


#endif //TP4_TOWERDEFENSE_LISTENER_H
