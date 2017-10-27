#ifndef TP4_TOWERDEFENSE_GAMEWINDOWHANDLER_H
#define TP4_TOWERDEFENSE_GAMEWINDOWHANDLER_H

#include "../common/Thread.h"

class GameWindowHandler : public Thread {
public:
    GameWindowHandler();
    virtual ~GameWindowHandler();

    void run();
};


#endif //TP4_TOWERDEFENSE_GAMEWINDOWHANDLER_H
