#ifndef TP4_ACCESS_TOWERDEFENSE_LISTENER_H
#define TP4_ACCESS_TOWERDEFENSE_LISTENER_H

#include "../common/Thread.h"
#include "../common/Socket.h"
#include "GameAccessWindow.h"
#include "../common/SharedBuffer.h"
#include "Notificable.h"

class Listener : public Thread {
private:
    Socket *server;
    Notificable &notificable;
    SharedBuffer &sharedBuffer;

public:
    Listener(Socket *server,
             Notificable &b,
             SharedBuffer &bfr);

    virtual ~Listener();

    void run();
};


#endif //TP4_ACCESS_TOWERDEFENSE_LISTENER_H
