#ifndef TP4_TOWERDEFENSE_ACTORTOWEREARTH_H
#define TP4_TOWERDEFENSE_ACTORTOWEREARTH_H


#include "ActorTower.h"

class ActorTowerEarth : public ActorTower {

public:
    explicit ActorTowerEarth(int id);
    ~ActorTowerEarth();

    bool upgradeDamage() override;

    bool upgradeRange() override;

    void doAttack(Horde *horde);

    std::string getClass() {
        return "Tierra";
    }
};


#endif //TP4_TOWERDEFENSE_ACTORTOWEREARTH_H
