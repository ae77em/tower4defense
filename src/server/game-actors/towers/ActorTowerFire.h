#ifndef TP4_TOWERDEFENSE_ACTORTOWERFIRE_H
#define TP4_TOWERDEFENSE_ACTORTOWERFIRE_H


#include "ActorTower.h"

class ActorTowerFire : public ActorTower {
public:
    ActorTowerFire(int id);
    ~ActorTowerFire();

    bool upgradeDamage() override;
};


#endif //TP4_TOWERDEFENSE_ACTORTOWERFIRE_H
