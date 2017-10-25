#ifndef TP4_TOWERDEFENSE_LISTENER_H
#define TP4_TOWERDEFENSE_LISTENER_H

#include "IntermediateBuffer.h"
#include "../common/Thread.h"
#include "../common/Socket.h"

class ClientListener : public Thread {
private:
    Socket &server;
    IntermediateBuffer &buffer;

public:
    ClientListener(Socket &server, IntermediateBuffer &b);

    virtual ~ClientListener();

    void run();

};


#endif //TP4_TOWERDEFENSE_LISTENER_H
