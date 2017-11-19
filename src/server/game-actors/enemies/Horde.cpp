#include "Horde.h"
#include "../../../common/Protocol.h"
#include "../../../sdl/Constants.h"
#include <vector>

void Horde::setEnemies(std::vector<ActorEnemy *> e){
    enemies = e;
}

std::vector<ActorEnemy *> Horde::getEnemies(){
    return enemies;
}

void Horde::addEnemy(ActorEnemy *enemy) {
    enemies.push_back(enemy);
}

Horde *Horde::createHorde(int enemyType, int amount, std::vector<Point> path){
    Horde *toReturn = new Horde();

    for (int i = 0; i < amount ; ++i){
        //hay que setear el desfasaje en los caminos, que cada bicho arranque
        // un poco mas atras qeu los demnas
        ActorEnemy *enemy = new ActorEnemy();

        enemy->setPath(path);
        enemy->setId(i);
        enemy->setCurrentPathPosition(-i * CARTESIAN_TILE_WIDTH / 2);

        toReturn->addEnemy(enemy);
    }

    return toReturn;
}

Horde::Horde(int t) : type(t) { }

Horde::Horde() { }

Horde::~Horde() {}

