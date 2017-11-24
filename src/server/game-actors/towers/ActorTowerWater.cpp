#include <cmath>
#include "ActorTowerWater.h"
#include "../../../sdl/Constants.h"

ActorTowerWater::ActorTowerWater(int id) : ActorTower(id) {
    range = 3;
    reach = 0;
    shotDamage = 4;
    shotSecondsGap = 3;
    isShooting = false;
    lastShotTime = 0;
    slowDownPercentaje = 0.25;
    experiencePoints = 0;
}

ActorTowerWater::~ActorTowerWater() { }

bool ActorTowerWater::upgradeDamage(){
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double damageUpgradePoints = pow(1.5, shotDamageLevel) * 100;

    // si tiene puntos suficientes
    if (damageUpgradePoints <= experiencePoints){
        // aplicar upgrade
        shotDamage += 4;
        upgraded = true;
    }

    return upgraded;
}

