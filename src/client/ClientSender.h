#ifndef TP4_TOWERDEFENSE_CLIENTSENDER_H
#define TP4_TOWERDEFENSE_CLIENTSENDER_H


#include <Thread.h>
#include "IntermediateBuffer.h"

class ClientSender : public Thread {
private:
    std::string host;
    uint16_t port;
    IntermediateBuffer &buffer;
    Socket server;

public:
    ClientSender(std::string h, uint16_t p, IntermediateBuffer &b);

    virtual ~ClientSender();

    void run();
};


#endif //TP4_TOWERDEFENSE_CLIENTSENDER_H
