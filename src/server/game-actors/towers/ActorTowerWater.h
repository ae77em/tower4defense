#ifndef TP4_TOWERDEFENSE_ACTORTOWERWATER_H
#define TP4_TOWERDEFENSE_ACTORTOWERWATER_H


#include "ActorTower.h"

class ActorTowerWater : public ActorTower {
public:
    explicit ActorTowerWater(int id);
    ~ActorTowerWater();

    bool upgradeDamage();
};


#endif //TP4_TOWERDEFENSE_ACTORTOWERWATER_H
