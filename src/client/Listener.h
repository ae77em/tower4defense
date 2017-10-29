#ifndef TP4_CLIENT_TOWERDEFENSE_LISTENER_H
#define TP4_CLIENT_TOWERDEFENSE_LISTENER_H

#include "../common/SharedBuffer.h"
#include "../common/Thread.h"
#include "../common/Socket.h"

class Listener : public Thread {
private:
    Socket &server;
    SharedBuffer &buffer;

public:
    Listener(Socket &server, SharedBuffer &b);

    virtual ~Listener();

    void run();
};


#endif //TP4_CLIENT_TOWERDEFENSE_LISTENER_H
