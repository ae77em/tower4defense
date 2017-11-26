#include <cmath>
#include "ActorTowerEarth.h"
#include "Constants.h"

ActorTowerEarth::ActorTowerEarth(int id) : ActorTower(id) {
    range = 2;
    reach = 0;
    shotDamage = 20;
    shotSecondsGap = 1;
    isShooting = false;
    lastShotTime = 0;
    slowdownPercentaje = 0.0;
    experiencePoints = 0;
}

ActorTowerEarth::~ActorTowerEarth() { }


bool ActorTowerEarth::upgradeDamage(){
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double damageUpgradePoints = pow(1.5, shotDamageLevel) * 100;

    // si tiene puntos suficientes
    if (damageUpgradePoints <= experiencePoints){
    // aplicar upgrade
        shotDamage += 10;
        ++shotDamageLevel;
        upgraded = true;
    }

    return upgraded;
}

bool ActorTowerEarth::upgradeRange() {
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double rangeUpgradePoints = pow(2, rangeLevel) * 500;

    // si tiene puntos suficientes
    if (rangeUpgradePoints <= experiencePoints){
        // aplicar upgrade
        ++range;
        ++rangeLevel;
        upgraded = true;
    }

    return upgraded;
}

void ActorTowerEarth::doAttack(ActorHorde *horde) {
    if (!horde->itIsAir()) {
        ActorTower::doAttack(horde);
    }
}
