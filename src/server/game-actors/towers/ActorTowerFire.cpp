#include <cmath>
#include "ActorTowerFire.h"
#include "../../../sdl/Constants.h"

ActorTowerFire::ActorTowerFire(int id) : ActorTower(id) {
    range = 3;
    reach = 1;
    shotDamage = 6;
    shotSecondsGap = 3;
    isShooting = false;
    lastShotTime = 0;
    slowDownPercentaje = 0.0;
    experiencePoints = 0;
}

ActorTowerFire::~ActorTowerFire() { }

bool ActorTowerFire::upgradeDamage(){
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double damageUpgradePoints = pow(1.5, shotDamageLevel) * 100;

    // si tiene puntos suficientes
    if (damageUpgradePoints <= experiencePoints){
        // aplicar upgrade
        shotDamage += 6;
        reachDamage += 3;
        upgraded = true;
    }

    return upgraded;
}