#ifndef TP4_TOWERDEFENSE_CLIENTSENDER_H
#define TP4_TOWERDEFENSE_CLIENTSENDER_H

#include "SharedBuffer.h"
#include "../common/Socket.h"
#include "../common/Thread.h"

class Sender : public Thread {
private:
    Socket server;
    SharedBuffer &buffer;

public:
    Sender(Socket &server, SharedBuffer &b);

    virtual ~Sender();

    void run();
};


#endif //TP4_TOWERDEFENSE_CLIENTSENDER_H
