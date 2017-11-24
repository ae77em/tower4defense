#include <cmath>
#include "ActorTowerEarth.h"
#include "../../../sdl/Constants.h"

ActorTowerEarth::ActorTowerEarth(int id) : ActorTower(id) {
    range = 2;
    reach = 0;
    shotDamage = 20;
    shotSecondsGap = 1;
    isShooting = false;
    lastShotTime = 0;
    slowDownPercentaje = 0.0;
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
