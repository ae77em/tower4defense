#include "ActorTower.h"
#include "../../../sdl/Constants.h"
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>

/* Descomentar la siguiente linea para activar salida de debug */
//#define LOG

ActorTower::ActorTower() {
    std::cout << "cree ActorTower" << std::endl;
    initialize();
}

ActorTower::ActorTower(int id) {
    this->id = id;
    initialize();
}

void ActorTower::initialize() {
    range = 2;
    reach = 1;
    shotSecondsGap = 3;
    isShooting = false;
    lastShotTime = 0;
    shotDamage = 50;
    slowdownPercentaje = 0.0;
    slowdownDuration = 0;
    experiencePoints = 0;

    rangeLevel = 1;
    shotDamageLevel = 1;
    reachLevel = 1;
    slowdownLevel = 1;

    collisionCircle.r = getCollisionCircleRadio();

    std::cout
            << "collision circle inicializado: "
            << collisionCircle.r
            << std::endl;
}

int ActorTower::getCollisionCircleRadio() {
    return (CARTESIAN_TILE_WIDTH * 0.5) + (range * CARTESIAN_TILE_WIDTH);
}

void ActorTower::live() {}

void ActorTower::shiftColliders() {
    // tanto el box de idle como el de disparo tienen las mismas dimensiones,
    // así que es indistinto qué medida tomamos
    collisionCircle.x = currentPoint.x + CARTESIAN_TILE_WIDTH * 0.5;
    collisionCircle.y = currentPoint.y + CARTESIAN_TILE_HEIGHT * 0.5;
}

Circle &ActorTower::getCollisionCircle() {
    return collisionCircle;
}

int ActorTower::getShotDamage(ActorEnemy *enemy) {
    return shotDamage;
}

void ActorTower::setIsShooting(bool isShoot) {
    isShooting = isShoot;
}

void ActorTower::sumExperiencePoints(int points) {
    experiencePoints += points;
}

std::string ActorTower::getClass() {
    return "Tower";
}

int ActorTower::getId() {
    return id;
}

void ActorTower::setId(int aId) {
    id = aId;
}

bool ActorTower::itIsShooting() {
    return isShooting;
}

void ActorTower::attack(Horde *horde) {
    doAttack(horde);
}

void ActorTower::doAttack(Horde *horde) {
    std::vector<ActorEnemy *> enemies = horde->getEnemies();

    for (ActorEnemy *enemy : enemies) {
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
        } else {
            isShooting = false;
        }
    }
}

void ActorTower::shootTo(ActorEnemy *pEnemy) {
    if (pEnemy->getIsAlive()) {
        doShootTo(pEnemy);
    }
}

void ActorTower::doShootTo(ActorEnemy *pEnemy) {
    std::cout << "daño al enemigo con " << std::to_string(shotDamage) <<
              std::endl;
    int damageAmount = getShotDamage(pEnemy);
    int expPoints = pEnemy->receiveDamage(damageAmount);
    sumExperiencePoints(expPoints);
    updateLastShotTime();
}

bool ActorTower::isReadyToShoot() const {
    time_t now;
    time(&now);
    return now - lastShotTime >= shotSecondsGap;
}

void ActorTower::setPosition(int x, int y) {
    currentPoint.x = x;
    currentPoint.y = y;

    shiftColliders();
}

ActorTower::~ActorTower() {}

std::string ActorTower::getClassInfo() {
    std::string toReturn = "Clase: ";
    toReturn.append(getClass());
    return toReturn;
}

std::string ActorTower::getExperiencePointsInfo() {
    std::string toReturn = "Puntos ";
    toReturn.append(std::to_string(experiencePoints));
    return toReturn;
}

std::string ActorTower::getRangeInfo() {
    std::string toReturn = "Rango: ";
    toReturn.append(std::to_string(range));
    toReturn.append(" (Nivel ");
    toReturn.append(std::to_string(rangeLevel));
    toReturn.append(")");

    return toReturn;
}

std::string ActorTower::getReachInfo() {
    std::string toReturn = "Alcance: ";
    toReturn.append(std::to_string(reach));
    toReturn.append(" (Nivel ");
    toReturn.append(std::to_string(reachLevel));
    toReturn.append(")");

    return toReturn;
}

std::string ActorTower::getSlowDownPercentajeInfo() {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << slowdownPercentaje;
    std::string slowdownStr = stream.str();
    std::string toReturn = "Ralentización: ";
    toReturn.append(slowdownStr);
    toReturn.append(" (Nivel ");
    toReturn.append(std::to_string(slowdownLevel));
    toReturn.append(")");

    return toReturn;
}

std::string ActorTower::getShotDamageInfo() {
    std::cout << "obtuve info daño BASE" << std::endl;
    std::string toReturn = "Daño: ";
    toReturn.append(std::to_string(shotDamage));
    toReturn.append(" (Nivel ");
    toReturn.append(std::to_string(shotDamageLevel));
    toReturn.append(")");

    return toReturn;
}

void ActorTower::updateLastShotTime() {
    time_t now;
    time(&now);
    lastShotTime = now;
}

bool ActorTower::upgradeDamage() {
    return false;
}

bool ActorTower::upgradeRange() {
    return false;
}

bool ActorTower::upgradeReach() {
    return false;
}

bool ActorTower::upgradeSlowdown() {
    return false;
}

