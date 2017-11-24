#include <cmath>
#include "ActorTowerFire.h"
#include "../../../sdl/Constants.h"

ActorTowerFire::ActorTowerFire() : ActorTower() {
    std::cout << "cree fuegoooo" << std::endl;
    initialize();
}

ActorTowerFire::ActorTowerFire(int aid) : ActorTower(aid) {
    initialize();
}

void ActorTowerFire::initialize() {
    std::cout << "inicialicé fuegoooo" << std::endl;
    range = 3;
    reach = 1;
    shotDamage = 6;
    shotSecondsGap = 3;
    isShooting = false;
    lastShotTime = 0;
    slowdownPercentaje = 0.0;
    experiencePoints = 0;
}

ActorTowerFire::~ActorTowerFire() { }

std::string ActorTowerFire::getShotDamageInfo() {
    std::cout << "obtuve info daño fuegoooo" << std::endl;
    std::string toReturn = "Daño objetivo: ";
    toReturn.append(std::to_string(shotDamage));
    toReturn.append(", Daño alcance: ");
    toReturn.append(std::to_string(reachDamage));
    toReturn.append(" (Nivel ");
    toReturn.append(std::to_string(shotDamageLevel));
    toReturn.append(")");

    return toReturn;
}

bool ActorTowerFire::upgradeDamage(){
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double damageUpgradePoints = pow(1.5, shotDamageLevel) * 100;

    // si tiene puntos suficientes
    if (damageUpgradePoints <= experiencePoints){
        // aplicar upgrade
        shotDamage += 6;
        reachDamage += 3;
        ++shotDamageLevel;
        upgraded = true;
    }

    return upgraded;
}

bool ActorTowerFire::upgradeRange() {
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

bool ActorTowerFire::upgradeReach() {
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double rangeUpgradePoints = pow(2, rangeLevel) * 500;

    // si tiene puntos suficientes
    if (rangeUpgradePoints <= experiencePoints){
        // aplicar upgrade
        ++reach;
        ++reachLevel;
        upgraded = true;
    }

    return upgraded;
}


