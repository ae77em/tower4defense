#ifndef TP4_ACCESS_TOWERDEFENSE_SENDER_H
#define TP4_ACCESS_TOWERDEFENSE_SENDER_H

#include "../common/Socket.h"
#include "../common/Thread.h"
#include "../common/SharedBuffer.h"
#include "GameAccess.h"

class SenderAccess : public Thread {
private:
    Socket server;
    SharedBuffer &buffer;

public:
    SenderAccess(Socket &server, SharedBuffer &b);

    virtual ~SenderAccess();

    void run();
};


#endif //TP4_ACCESS_TOWERDEFENSE_SENDER_H
