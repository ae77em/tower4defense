#ifndef TP4_TOWERDEFENSE_ACTORTOWERAIR_H
#define TP4_TOWERDEFENSE_ACTORTOWERAIR_H


#include "ActorTower.h"

class ActorTowerAir : public ActorTower {
public:
    ActorTowerAir();
    ActorTowerAir(int id);
    ~ActorTowerAir();

    int getShotDamage(ActorEnemy enemy) override;

    bool upgradeDamage() override;

    bool upgradeRange() override;

private:
    int shotAirDamage;

    int id;

    std::string getShotDamageInfo();
};


#endif //TP4_TOWERDEFENSE_ACTORTOWERAIR_H
