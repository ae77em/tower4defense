#include <cmath>
#include "ActorTowerAir.h"
#include "../../../sdl/Constants.h"

ActorTowerAir::ActorTowerAir(int id) : ActorTower(id) {
    range = 5;
    reach = 0;
    shotDamage = 2;
    shotAirDamage = 10;
    shotSecondsGap = 5;
    isShooting = false;
    lastShotTime = 0;
    slowDownPercentaje = 0.0;
    experiencePoints = 0;
}

ActorTowerAir::~ActorTowerAir() { }

int ActorTowerAir::getShotDamage(ActorEnemy enemy) {
    if (enemy.itIsAir()){
        return shotAirDamage;
    } else {
        return shotDamage;
    }
}

bool ActorTowerAir::upgradeDamage(){
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double damageUpgradePoints = pow(1.5, shotDamageLevel) * 100;

    // si tiene puntos suficientes
    if (damageUpgradePoints <= experiencePoints){
        // aplicar upgrade
        shotDamage += 2;
        shotAirDamage += 10;
        upgraded = true;
    }

    return upgraded;
}
