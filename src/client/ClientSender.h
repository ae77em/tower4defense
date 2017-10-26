#ifndef TP4_TOWERDEFENSE_CLIENTSENDER_H
#define TP4_TOWERDEFENSE_CLIENTSENDER_H


#include "../common/Thread.h"

class ClientSender : public Thread {

    ClientSender();

    virtual ~ClientSender();

    void run();
};


#endif //TP4_TOWERDEFENSE_CLIENTSENDER_H
