#ifndef TP4_ACCESS_TOWERDEFENSE_SENDER_H
#define TP4_ACCESS_TOWERDEFENSE_SENDER_H

#include "../common/Socket.h"
#include "../common/Thread.h"
#include "../common/SharedBuffer.h"
#include "GameAccess.h"

class Sender : public Thread {
private:
    Socket server;
    SharedBuffer &buffer;

public:
    Sender(Socket &server, SharedBuffer &b);

    virtual ~Sender();

    void run();
};


#endif //TP4_ACCESS_TOWERDEFENSE_SENDER_H
