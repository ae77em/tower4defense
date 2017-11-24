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
    slowdownPercentaje = 0.0;
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

std::string ActorTowerAir::getShotDamageInfo() {
    std::string toReturn = "Daño no aéreo: ";
    toReturn.append(std::to_string(shotDamage));
    toReturn.append(", Daño aéreo: ");
    toReturn.append(std::to_string(shotAirDamage));
    toReturn.append(" (Nivel ");
    toReturn.append(std::to_string(shotDamageLevel));
    toReturn.append(")");

    return toReturn;
}

ActorTowerAir::ActorTowerAir() {
    initialize();
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
        ++shotDamageLevel;
        upgraded = true;
    }

    return upgraded;
}

bool ActorTowerAir::upgradeRange() {
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

