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
    slowdownDuration = 2;
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

void ActorTowerWater::doAttack(Horde *horde) {
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
            applySlowdownTo(enemy);
        } else {
            isShooting = false;
        }
    }
}

void ActorTowerWater::applySlowdownTo(ActorEnemy *pEnemy) {
    pEnemy->setSlowdown(slowdownPercentaje, slowdownDuration);
}
