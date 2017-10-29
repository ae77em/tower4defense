#ifndef TP4_ACCESS_TOWERDEFENSE_LISTENER_H
#define TP4_ACCESS_TOWERDEFENSE_LISTENER_H

#include "../common/Thread.h"
#include "../common/Socket.h"
#include "GameAccess.h"

class Listener : public Thread {
private:
    Socket &server;
    GameAccess &gameAccess;

public:
    Listener(Socket &server, GameAccess &b);

    virtual ~Listener();

    void run();
};


#endif //TP4_ACCESS_TOWERDEFENSE_LISTENER_H
