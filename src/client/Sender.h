#ifndef TP4_CLIENT_TOWERDEFENSE_SENDER_H
#define TP4_CLIENT_TOWERDEFENSE_SENDER_H

#include "SharedBuffer.h"
#include "Socket.h"
#include "Thread.h"

class Sender : public Thread {
private:
    Socket *server;
    SharedBuffer &buffer;

public:
    Sender(Socket *server, SharedBuffer &b);

    virtual ~Sender();

    void run();

    void shutdown();
};


#endif //TP4_CLIENT_TOWERDEFENSE_SENDER_H
