#ifndef TP4_TOWERDEFENSE_ACTORTOWERAIR_H
#define TP4_TOWERDEFENSE_ACTORTOWERAIR_H


#include "ActorTower.h"

class ActorTowerAir : public ActorTower {
public:
    explicit ActorTowerAir(int id);
    ~ActorTowerAir();

    int getShotDamage(ActorEnemy enemy) override;

    bool upgradeDamage() override;

private:
    int shotAirDamage;
};


#endif //TP4_TOWERDEFENSE_ACTORTOWERAIR_H
