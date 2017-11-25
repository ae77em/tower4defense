#ifndef TP4_TOWERDEFENSE_ACTORTOWERWATER_H
#define TP4_TOWERDEFENSE_ACTORTOWERWATER_H


#include "ActorTower.h"

class ActorTowerWater : public ActorTower {
public:
    explicit ActorTowerWater(int id);
    ~ActorTowerWater();

    bool upgradeDamage() override;

    bool upgradeRange() override;

    bool upgradeSlowdown() override;

    void doAttack(Horde *horde);

    std::string getClass() {
        return "Agua";
    }

    void applySlowdownTo(ActorEnemy *pEnemy);
};


#endif //TP4_TOWERDEFENSE_ACTORTOWERWATER_H
