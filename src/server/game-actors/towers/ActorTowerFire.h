#ifndef TP4_TOWERDEFENSE_ACTORTOWERFIRE_H
#define TP4_TOWERDEFENSE_ACTORTOWERFIRE_H


#include "ActorTower.h"

class ActorTowerFire : public ActorTower {
public:
    ActorTowerFire();
    explicit ActorTowerFire(int id);
    ~ActorTowerFire();

    bool upgradeDamage() override;

    std::string getShotDamageInfo() override;

    void initialize() override;
private:
    int id;
};


#endif //TP4_TOWERDEFENSE_ACTORTOWERFIRE_H
