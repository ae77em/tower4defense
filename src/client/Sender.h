#ifndef TP4_CLIENT_TOWERDEFENSE_SENDER_H
#define TP4_CLIENT_TOWERDEFENSE_SENDER_H

#include "../common/SharedBuffer.h"
#include "../common/Socket.h"
#include "../common/Thread.h"

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
