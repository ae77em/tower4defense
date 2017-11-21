#ifndef TP4_TOWERDEFENSE_GAMEACTIONPUTTOWER_H
#define TP4_TOWERDEFENSE_GAMEACTIONPUTTOWER_H

#include "../GameAction.h"

class GameActionPutTower : public GameAction {
public:
    GameActionPutTower(std::string name, int x, int y, int typeOfTower);
    ~GameActionPutTower() override;

    int getX() const;
    int getY() const;
    int getTypeOfTower() const;

private:
    int typeOfTower;
    int x;
    int y;
};


#endif //TP4_TOWERDEFENSE_GAMEACTIONPUTTOWER_H
