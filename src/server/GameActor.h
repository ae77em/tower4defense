#ifndef TP4_TOWERDEFENSE_SERVER_GAMEACTOR_H
#define TP4_TOWERDEFENSE_SERVER_GAMEACTOR_H

#include <iostream>
#include <string>

struct ActorRectT {
    int x;
    int y;
    int w;
    int h;
};

class GameActor {
public:
    GameActor();

    virtual ~GameActor();
    //deberia ir a otra clase superior
    virtual std::string getClass() = 0;

    virtual void live() = 0;
    virtual int getXPosition() = 0;
    virtual int getYPosition() = 0;
    virtual int getEnergy() = 0;
    virtual int getDirection() = 0;
    virtual int getId() = 0;
    virtual ActorRectT getRect() = 0;
};



#endif //TP4_TOWERDEFENSE_GAMEACTOR_H
