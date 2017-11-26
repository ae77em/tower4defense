#include "ActorHorde.h"
#include "Protocol.h"
#include "Constants.h"
#include "ActorEnemyAbmonible.h"
#include "ActorEnemyBloodHawk.h"
#include "ActorEnemyGoatman.h"
#include "ActorEnemyGreenDaemon.h"
#include "ActorEnemySpectre.h"
#include "ActorEnemyZombie.h"
#include <vector>

ActorHorde::ActorHorde(int t) : type(t) {
    initialize();
}

ActorHorde::ActorHorde() {
    initialize();
}

void ActorHorde::initialize(){
    isAlive = true;
    timeOfDeath = 0;
    mustSendData = true;
}

ActorHorde::~ActorHorde() = default;

void ActorHorde::setEnemies(std::vector<ActorEnemy *> e){
    enemies = e;
}

std::vector<ActorEnemy *> ActorHorde::getEnemies(){
    return enemies;
}

bool ActorHorde::itIsAir(){
    bool isAir = false;
    if (enemies.size() > 0) {
        isAir = enemies.at(0)->itIsAir();
    }
    return isAir;
}

void ActorHorde::addEnemy(ActorEnemy *enemy) {
    enemies.push_back(enemy);
}

ActorHorde *ActorHorde::createHorde(int enemyType, int amount, std::vector<Point> path){
    ActorHorde *toReturn = new ActorHorde();

    std::vector<Point> actualPath;
    int x, y;

    for (Point pathPoint : path){
        x = pathPoint.x * CARTESIAN_TILE_WIDTH;
        y = pathPoint.y * CARTESIAN_TILE_HEIGHT;
        actualPath.push_back(Point(x,y));
    }

    for (int i = 0; i < amount ; ++i){
        ActorEnemy *enemy = getEnemyByType(enemyType);

        enemy->setPath(actualPath);
        enemy->setId(i);

        /* Enemigo en posicion negativa simula delay de creacion
         * (vease ActorEnemy.h)
         */
        enemy->setCurrentPathPosition(-i * CARTESIAN_TILE_WIDTH / 2);

        toReturn->addEnemy(enemy);
    }

    return toReturn;
}

ActorEnemy *ActorHorde::getEnemyByType(int type) {
    ActorEnemy *enemy = nullptr;
    switch (type){
        case ENEMY_ABMONIBLE:{
            enemy = new ActorEnemyAbmonible();
            break;
        }
        case ENEMY_BLOOD_HAWK:{
            enemy = new ActorEnemyBloodHawk();
            break;
        }
        case ENEMY_GOATMAN:{
            enemy = new ActorEnemyGoatman();
            break;
        }
        case ENEMY_GREEN_DAEMON:{
            enemy = new ActorEnemyGreenDaemon();
            break;
        }
        case ENEMY_SPECTRE:{
            enemy = new ActorEnemySpectre();
            break;
        }
        case ENEMY_ZOMBIE:{
            enemy = new ActorEnemyZombie();
            break;
        }
        default:{
            enemy = new ActorEnemy();
        }
    }
    return enemy;
}

void ActorHorde::setIsAlive(bool isAlive) {
    ActorHorde::isAlive = isAlive;
    setTimeOfDeath();
}

bool ActorHorde::itIsAlive() {
    return isAlive;
}

void ActorHorde::setTimeOfDeath() {
    time_t now;
    time(&now);
    timeOfDeath = now;
}

bool ActorHorde::shouldSendMoreData(){
    /* Si previamente me setearon, ya no tengo que mandar más info de mi
     * estado. Si no, verifico que sí tenga que enviar. El mustSendData se
     * setearía una sola vez.
     * */
    if (mustSendData && !isAlive){
        if(timeOfDeath != 0){
            time_t now;
            time(&now);
            if (now - timeOfDeath > 2){
                mustSendData = false;
            }
        }
    }

    return mustSendData;
}
