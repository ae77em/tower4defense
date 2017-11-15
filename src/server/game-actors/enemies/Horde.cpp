#include "Horde.h"

void Horde::setEnemies(std::vector<ActorEnemy *> e){
    enemies = e;
}

std::vector<ActorEnemy *> Horde::getEnemies(){
    return enemies;
}

void Horde::addEnemy(ActorEnemy *enemy) {
    enemies.push_back(enemy);
}

Horde::Horde() {}

Horde::~Horde() {}
