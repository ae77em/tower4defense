#ifndef TP4_TOWERDEFENSE_GAMEACTOR_H
#define TP4_TOWERDEFENSE_GAMEACTOR_H


class GameActor {
public:
    GameActor();
    //deberia ir a otra clase superior
    virtual std::string getClass() = 0;

    virtual void live() = 0;
    virtual int getXPosition() = 0;
    virtual int getYPosition() = 0;
    virtual int getEnergy() = 0;

};


#endif //TP4_TOWERDEFENSE_GAMEACTOR_H
