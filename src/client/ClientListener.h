//
// Created by esteban on 24/10/17.
//

#ifndef TP4_TOWERDEFENSE_LISTENER_H
#define TP4_TOWERDEFENSE_LISTENER_H


#include "../common/Thread.h"

class ClientListener : public Thread {

    ClientListener();

    virtual ~ClientListener();

    void run();

};


#endif //TP4_TOWERDEFENSE_LISTENER_H
