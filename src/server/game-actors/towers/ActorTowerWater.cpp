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
    slowdownPercentaje = 0.25;
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
        ++shotDamageLevel;
        upgraded = true;
    }

    return upgraded;
}

bool ActorTowerWater::upgradeRange() {
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double rangeUpgradePoints = pow(2, rangeLevel) * 100;

    // si tiene puntos suficientes
    if (rangeUpgradePoints <= experiencePoints){
        // aplicar upgrade
        ++range;
        ++rangeLevel;
        upgraded = true;
    }

    return upgraded;
}

bool ActorTowerWater::upgradeSlowdown() {
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double slowdownUpgradePoints = pow(2, rangeLevel) * 100;

    // si tiene puntos suficientes
    if (slowdownUpgradePoints <= experiencePoints){
        // aplicar upgrade
        if (slowdownPercentaje < 1.0){
            slowdownPercentaje += 0.25; // sumo porcentaje...
        }
        if (slowdownPercentaje > 1.0){
            slowdownPercentaje = 1.0; // si por errores de redondeo me pase
        }
        ++slowdownDuration;
        ++slowdownLevel;
        upgraded = true;
    }

    return upgraded;
}

