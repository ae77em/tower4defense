#ifndef TP4_TOWERDEFENSE_ACTORTOWERFIRE_H
#define TP4_TOWERDEFENSE_ACTORTOWERFIRE_H


#include "ActorTower.h"

class ActorTowerFire : public ActorTower {
public:
    ActorTowerFire();
    explicit ActorTowerFire(int id);
    ~ActorTowerFire();

    std::string getShotDamageInfo() override;

    void initialize() override;

    bool upgradeDamage() override;

    bool upgradeRange() override;

    bool upgradeReach() override;
private:
    int id;
};


#endif //TP4_TOWERDEFENSE_ACTORTOWERFIRE_H
