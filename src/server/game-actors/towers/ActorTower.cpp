#include "ActorTower.h"
#include "../../../sdl/Constants.h"
#include <string>

ActorTower::ActorTower() {
    initialize();
}

void ActorTower::initialize() {
    range = 2;
    reach = 1;
    shotDamage = 50;
    shotMsTimeGap = 3000; //miliseconds
    slowDownPercentaje = 0.0;
    collisionCircle.r = (CARTESIAN_TILE_WIDTH / 2) * range;
}

void ActorTower::live(){ }

void ActorTower::shiftColliders() {
    // tanto el box de idle como el de disparo tienen las mismas dimensiones,
    // así que es indistinto qué medida tomamos
    // TODO verificar estas asignaciones...
    collisionCircle.x = currentPoint.x;
    collisionCircle.y = currentPoint.y;
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
    std::vector<ActorEnemy*> enemies = horde->getEnemies();

    for (ActorEnemy *enemy : enemies){
        Circle &collisionCircleEnemy = enemy->getCollisionCircle();

        if (collisionCircle.hasCollisionWith(collisionCircleEnemy)){
            std::cout
                    << " torre "
                    << id
                    << " dispara a monstruo "
                    << enemy->getId()
                    << std::endl;
            isShooting = true;
            shootTo(enemy);
        } else {
            // si no hay colisión:
            // estado: iddle
            isShooting = false;
        }
    }
}

ActorRectT ActorTower::getRect() {
    return rect;
}

void ActorTower::shootTo(ActorEnemy *pEnemy) {
    time_t now;
    time(&now);
    if (now - lastShotTime >= shotMsTimeGap){
        // si puedo disparar le disparo, esto es, le saco toda la vida que puedo
        if (pEnemy->itIsAlive()){
            std::cout << "disparé..." << std::endl;
            doShootTo(pEnemy);
        }
    }
}

void ActorTower::doShootTo(ActorEnemy *pEnemy) {
    sumExperiencePoints(pEnemy->receiveDamage(shotDamage));
}

void ActorTower::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

ActorTower::~ActorTower() { }

int ActorTower::getSlowDownPercentaje() {
    return slowDownPercentaje;
}

void ActorTower::setSlowDownPercentaje(double perc) {
    slowDownPercentaje = perc;
}

ActorTower::ActorTower(int id) {
    this->id = id;
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

