#include "Horde.h"
#include "../../../common/Protocol.h"
#include "../../../sdl/Constants.h"
#include "ActorEnemyAbmonible.h"
#include "ActorEnemyBloodHawk.h"
#include "ActorEnemyGoatman.h"
#include "ActorEnemyGreenDaemon.h"
#include "ActorEnemySpectre.h"
#include "ActorEnemyZombie.h"
#include <vector>

Horde::Horde(int t) : type(t) {
    initialize();
}

Horde::Horde() {
    initialize();
}

void Horde::initialize(){
    isAlive = true;
    timeOfDeath = 0;
    mustSendData = true;
}

Horde::~Horde() = default;

void Horde::setEnemies(std::vector<ActorEnemy *> e){
    enemies = e;
}

std::vector<ActorEnemy *> Horde::getEnemies(){
    return enemies;
}

void Horde::addEnemy(ActorEnemy *enemy) {
    enemies.push_back(enemy);
}

//FIXME: enemyType es ignorado
Horde *Horde::createHorde(int enemyType, int amount, std::vector<Point> path){
    Horde *toReturn = new Horde();

    std::vector<Point> actualPath;
    int x, y;

    for (Point pathPoint : path){
        x = pathPoint.x * CARTESIAN_TILE_WIDTH;
        y = pathPoint.y * CARTESIAN_TILE_HEIGHT;
        actualPath.push_back(Point(x,y));
    }

    for (int i = 0; i < amount ; ++i){
        //hay que setear el desfasaje en los caminos, que cada bicho arranque
        // un poco mas atras qeu los demnas
        ActorEnemy *enemy = getEnemyByType(enemyType);

        enemy->setPath(actualPath);
        enemy->setId(i);
        enemy->setCurrentPathPosition(-i * CARTESIAN_TILE_WIDTH / 2);

        toReturn->addEnemy(enemy);
    }

    return toReturn;
}

ActorEnemy *Horde::getEnemyByType(int type) {
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

void Horde::setIsAlive(bool isAlive) {
    Horde::isAlive = isAlive;
    setTimeOfDeath();
}

bool Horde::itIsAlive() {
    return isAlive;
}

void Horde::setTimeOfDeath() {
    time_t now;
    time(&now);
    timeOfDeath = now;
}

bool Horde::shouldSendMoreData(){
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
