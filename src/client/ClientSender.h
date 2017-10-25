#ifndef TP4_TOWERDEFENSE_CLIENTSENDER_H
#define TP4_TOWERDEFENSE_CLIENTSENDER_H


#include <Thread.h>

class ClientSender : public Thread {

    ClientSender();

    virtual ~ClientSender();

    void run();
};


#endif //TP4_TOWERDEFENSE_CLIENTSENDER_H
