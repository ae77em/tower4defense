#ifndef TP4_TOWERDEFENSE_ACTORTOWERFIRE_H
#define TP4_TOWERDEFENSE_ACTORTOWERFIRE_H


#include "ActorTower.h"

class ActorTowerFire : public ActorTower {
public:
    ActorTowerFire();
    explicit ActorTowerFire(int id);
    ~ActorTowerFire();

    std::string getShotDamageInfo() override;

    void doAttack(ActorHorde *horde) override;

    void initialize() override;

    bool upgradeDamage() override;

    bool upgradeRange() override;

    bool upgradeReach() override;

    std::string getClass() {
        return "Fuego";
    }
private:
    int id;

    void damageNearbyEnemies(std::vector<ActorEnemy *> &enemies,
                             unsigned int currEnemyPos);

    void doReachDamageTo(ActorEnemy *pEnemy);
};


#endif //TP4_TOWERDEFENSE_ACTORTOWERFIRE_H
