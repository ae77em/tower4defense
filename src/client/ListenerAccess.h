#ifndef TP4_ACCESS_TOWERDEFENSE_LISTENER_H
#define TP4_ACCESS_TOWERDEFENSE_LISTENER_H

#include "../common/Thread.h"
#include "../common/Socket.h"
#include "GameAccess.h"

class ListenerAccess : public Thread {
private:
    Socket &server;
    GameAccess &gameAccess;

public:
    ListenerAccess(Socket &server, GameAccess &b);

    virtual ~ListenerAccess();

    void run();
};


#endif //TP4_ACCESS_TOWERDEFENSE_LISTENER_H
