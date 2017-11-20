#ifndef TP4_TOWERDEFENSE_ACTORTOWERAIR_H
#define TP4_TOWERDEFENSE_ACTORTOWERAIR_H


#include "ActorTower.h"

class ActorTowerAir : public ActorTower {
public:
    ActorTowerAir(int id);

    void initialize();

    int getShotDamage(ActorEnemy enemy);

};


#endif //TP4_TOWERDEFENSE_ACTORTOWERAIR_H
