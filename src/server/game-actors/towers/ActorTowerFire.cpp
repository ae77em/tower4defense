#include <cmath>
#include "ActorTowerFire.h"
#include "../../../sdl/Constants.h"

//#define LOG

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
    reachDamage = 3;
    shotSecondsGap = 3;
    isShooting = false;
    lastShotTime = 0;
    slowdownPercentaje = 0.0;
    experiencePoints = 0;
}

ActorTowerFire::~ActorTowerFire() {}

std::string ActorTowerFire::getShotDamageInfo() {
    std::string toReturn = "Daño objetivo: ";
    toReturn.append(std::to_string(shotDamage));
    toReturn.append(", Daño alcance: ");
    toReturn.append(std::to_string(reachDamage));
    toReturn.append(" (Nivel ");
    toReturn.append(std::to_string(shotDamageLevel));
    toReturn.append(")");

    return toReturn;
}

bool ActorTowerFire::upgradeDamage() {
    bool upgraded = false;

    // calculo puntos necesarios para el upgrade
    double damageUpgradePoints = pow(1.5, shotDamageLevel) * 100;

    // si tiene puntos suficientes
    if (damageUpgradePoints <= experiencePoints) {
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
    if (rangeUpgradePoints <= experiencePoints) {
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
    if (rangeUpgradePoints <= experiencePoints) {
        // aplicar upgrade
        ++reach;
        ++reachLevel;
        upgraded = true;
    }

    return upgraded;
}

void ActorTowerFire::doAttack(Horde *horde) {
    std::vector<ActorEnemy *> enemies = horde->getEnemies();

    for (unsigned i = 0; i < enemies.size(); ++i) {
        ActorEnemy *enemy = enemies[i];

        Circle &collisionCircleEnemy = enemy->getCollisionCircle();
#ifdef LOG
        std::cout << "enemy "
              << enemy->getId()
              << " is (col circle) in ("
              << enemy->getCollisionCircle().x
              << ", "
              << enemy->getCollisionCircle().y
              << ") - radio: "
              << enemy->getCollisionCircle().r
              << std::endl;
    std::cout << "tower "
              << std::to_string(id)
              << " is (col circle) in ("
              << std::to_string(collisionCircle.x)
              << ", "
              << std::to_string(collisionCircle.y)
              << ") - radio: "
              << std::to_string(collisionCircle.r)
            << " - radio calculated: "
            << std::to_string(getCollisionCircleRadio())
              << std::endl;
#endif
        if (collisionCircle.hasCollisionWith(collisionCircleEnemy)) {
            isShooting = true;
            shootTo(enemy);
            damageNearbyEnemies(enemies, i);
        } else {
            isShooting = false;
        }
    }
}

void ActorTowerFire::damageNearbyEnemies(std::vector<ActorEnemy *> &enemies,
                                         unsigned int currEnemyPos) {
    // si puedo disparar le disparo, esto es, le saco toda la vida que puedo
    ActorEnemy *pEnemy = nullptr;
    int posEnemyToReach = -1;
    for (int i = -reach; i <= reach; ++i){
        if (i != 0){ // el enemigo que sufrió el disparo no recibe daño extra
            // verifico no querer dañar un enemigo fuera del array !!!
            posEnemyToReach = currEnemyPos + i;
            if (currEnemyPos + i >= 0 && currEnemyPos + i < enemies.size()){
                pEnemy = enemies[posEnemyToReach];
                if (pEnemy->getIsAlive()){
                    std::cout
                            << "daño a enemigo cercano "
                            << i
                            << " con "
                            << reachDamage
                            << "puntos."
                            << std::endl;
                    doReachDamageTo(pEnemy);
                }
            }
        }
    }

}

void ActorTowerFire::doReachDamageTo(ActorEnemy *pEnemy) {
    int damageAmount = pEnemy->receiveDamage(reachDamage);
    int expPoints = pEnemy->receiveDamage(damageAmount);
    sumExperiencePoints(expPoints);
}



