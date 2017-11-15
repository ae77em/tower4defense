#include "ActorTower.h"
#include "../../../sdl/Constants.h"
#include <string>

ActorTower::ActorTower() { }

void ActorTower::initialize() {
    shotRatio = 2;
    shotDamage = 50;
    shotMsTimeGap = 3000; //miliseconds
    collisionCircle.r = (CARTESIAN_TILE_WIDTH / 2) * shotRatio;
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

int ActorTower::getShotDamage() {
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
    return std::__cxx11::string();
}

int ActorTower::getDirection() {
    return 0;
}

int ActorTower::getXPosition() {
    return 0;
}

int ActorTower::getYPosition() {
    return 0;
}

int ActorTower::getEnergy() {
    return 0;
}

int ActorTower::getId() {
    return 0;
}

bool ActorTower::itIsShooting() {
    return isShooting;
}
