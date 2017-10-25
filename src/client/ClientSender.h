#ifndef TP4_TOWERDEFENSE_CLIENTSENDER_H
#define TP4_TOWERDEFENSE_CLIENTSENDER_H

#include "IntermediateBuffer.h"
#include "../common/Socket.h"
#include "../common/Thread.h"

class ClientSender : public Thread {
private:
    Socket server;
    IntermediateBuffer &buffer;

public:
    ClientSender(Socket &server, IntermediateBuffer &b);

    virtual ~ClientSender();

    void run();
};


#endif //TP4_TOWERDEFENSE_CLIENTSENDER_H
