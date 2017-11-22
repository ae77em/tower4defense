#include "ActorTower.h"
#include "../../../sdl/Constants.h"
#include <string>

//#define LOG

ActorTower::ActorTower() {
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
    slowDownPercentaje = 0.0;
    experiencePoints = 0;

    levelRange = 1;
    levelDamage = 1;
    levelReach = 1;
    levelSlowdown = 1;

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

int ActorTower::getShotDamage(ActorEnemy enemy) {
    return shotDamage;
}

void ActorTower::setIsShooting(bool isShoot) {
    isShooting = isShoot;
}

void ActorTower::sumExperiencePoints(int points) {
    experiencePoints += points;
}

int ActorTower::getExperiencePoints() {
    return experiencePoints;
}

std::string ActorTower::getClass() {
    return "ActorTower";
}

int ActorTower::getId() {
    return id;
}

bool ActorTower::itIsShooting() {
    return isShooting;
}

void ActorTower::attack(Horde *horde) {
    if (isReadyToShoot()) {
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
                std::cout << "hubo colision" << std::endl;
                isShooting = true;
                shootTo(enemy);
            } else {
                isShooting = false;
            }
        }
    }
}

void ActorTower::shootTo(ActorEnemy *pEnemy) {
    // si puedo disparar le disparo, esto es, le saco toda la vida que puedo
    if (pEnemy->itIsAlive()) {
        doShootTo(pEnemy);
    }
}

bool ActorTower::isReadyToShoot() const {
    time_t now;
    time(&now);
    return now - lastShotTime >= shotSecondsGap;
}

void ActorTower::doShootTo(ActorEnemy *pEnemy) {
    std::cout << "daño al enemigo con " << std::to_string(shotDamage) <<
                                                                      std::endl;
    sumExperiencePoints(pEnemy->receiveDamage(shotDamage));
    updateLastShotTime();
}

void ActorTower::setPosition(int x, int y) {
    currentPoint.x = x;
    currentPoint.y = y;

    shiftColliders();
}

ActorTower::~ActorTower() {}

int ActorTower::getSlowDownPercentaje() {
    return slowDownPercentaje;
}

void ActorTower::setSlowDownPercentaje(double perc) {
    slowDownPercentaje = perc;
}

int ActorTower::getRange() {
    return range;
}

int ActorTower::getReach() {
    return 0;
}

std::string ActorTower::getRangeInfo() {
    return std::to_string(range);
}

std::string ActorTower::getReachInfo() {
    return std::to_string(reach);
}

std::string ActorTower::getSlowDownPercentajeInfo() {
    return std::to_string(slowDownPercentaje);
}

std::string ActorTower::getShotDamageInfo() {
    return std::to_string(shotDamage);
}

void ActorTower::updateLastShotTime() {
    time_t now;
    time(&now);
    lastShotTime = now;
}

