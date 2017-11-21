#include "GameActionPutTower.h"

#include <utility>

GameActionPutTower::GameActionPutTower(std::string name,
                                       int aTypeOfTower,
                                       int ax,
                                       int ay
                                       )
        : GameAction(std::move(name)) {
    x = ax;
    y = ay;
    typeOfTower = aTypeOfTower;
}

GameActionPutTower::~GameActionPutTower() = default;

int GameActionPutTower::getX() const {
    return x;
}

int GameActionPutTower::getY() const {
    return y;
}

int GameActionPutTower::getTypeOfTower() const {
    return typeOfTower;
}
