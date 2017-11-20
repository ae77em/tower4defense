#include "WorkerLoopGame.h"
#include "../common/Point.h"
#include "../sdl/Constants.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "game-actors/enemies/Horde.h"
#include "../common/MessageFactory.h"
#include "GameNotification.h"
#include "../common/model/Map.h"
#include "../common/Protocol.h"
#include "game-actors/towers/ActorTowerFire.h"
#include "game-actors/towers/ActorTowerWater.h"
#include "game-actors/towers/ActorTowerAir.h"
#include "game-actors/towers/ActorTowerEarth.h"
#include "../client/GamePlayWindow.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <map>
#include <string>
#include <list>
#include <vector>

//HAY QUE AGREGAR MUTEX PLAYERD PARA QUE BLOQUEE LA LISTA DE SERVER PLAYERS
//PUEDE SER QUE SE ESTE REMOVIENDO ALGUNO EN OTRO HILO Y LUEGO EXPLOTE
WorkerLoopGame::WorkerLoopGame(std::map<int, ServerPlayer *> &p,
                               std::list<GameAction *> &a,
                               std::mutex &m,
                               model::Map &mp) :
        players(p),
        actions(a),
        mutexActions(m),
        map(mp) {}

void WorkerLoopGame::run() {
    std::cout << "WorkerLoopGame: Hilo donde "
            "existe la partida arrancando" << std::endl;

    std::string statusGame;

    std::list<GameAction *> actionsGame;
    std::map<int, Horde *>::iterator hordeIt;

    bool gameFinish = false;
    std::string cause = "";

    while (!gameFinish) {

        if (isTimeToCreateHorde()) {
            createHordeAndNotify();
        }

        if (!actionsSuccessfullAttended(actionsGame)) {
            break;
        }

        /* Muevo los enemigos. */
        for (hordeIt = hordes.begin(); hordeIt != hordes.end(); ++hordeIt) {
            std::vector<ActorEnemy *> enemies = hordeIt->second->getEnemies();

            for (auto enemy : enemies) {
                enemy->advance();
                if (enemy->hasEndedThePath()) {
                    gameFinish = true;
                    notifyMatchLoose();
                }
            }
        }

        /* Hago actuar las torres. */
        for (auto tower : towers) {
            for (hordeIt = hordes.begin(); hordeIt != hordes.end(); ++hordeIt) {
                tower->attack(hordeIt->second);
            }
        }

        /* Obtengo el estado actual */
        statusGame = getGameStatus();

        /* Notifico el estado del juego a todos los jugadores. */
        for (auto it = players.begin(); it != players.end(); ++it) {
            it->second->sendData(statusGame);
        }

        /* Limpio la lista para no ejecutar request viejos. */
        actionsGame.clear();

        /* Poco menos de 60 fps. Lo dejo así para darle tiempo a los request
         * de los usuarios a impactarse de forma relativamente instantánea. */
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}

bool WorkerLoopGame::actionsSuccessfullAttended(
        std::list<GameAction *> &actionsGame) {
    bool actionSuccessful = true;
    mutexActions.lock();
    //GRONCHADA PARA REFACTORIZAR
    //IMPLEMENTAR CLASE QUE HAGA EL CIERRE DEL GAME LOOP

    /* Estos son los requests enviados por el cliente, por ejemplo,
     * poner torre... */
    for (GameAction *a : actions) {
        std::cout << a->action << "  " << std::endl;

        if (a->action.compare("game-explotion") == 0) {
            std::cout
                    << "WorkerLoopGame: salgo porque explote "
                    << std::endl;
            actionSuccessful = false;
            break;
        } else if (a->action.compare(STR_PUT_TOWER) == 0) {
            putTower(a);
        } else if (a->action.compare(STR_GET_TOWER_INFO) == 0) {
            // get tower info
        } else if (a->action.compare(STR_UPGRADE_TOWER) == 0) {
            // get upgrade tower
        }
        actionsGame.push_back(a);
    }

    actions.clear();

    mutexActions.unlock();

    return actionSuccessful;
}

void WorkerLoopGame::buildGameContext() {
    timeBetweenHordeCreation = 5000; //milisegundos
    timeLastHordeCreation = 0;

    hordeType.push_back((int) ENEMY_ZOMBIE);
    hordeType.push_back((int) ENEMY_GOATMAN);
    hordeType.push_back((int) ENEMY_SPECTRE);

    paths = map.getPaths();

    hordeId = 0;
}

std::string WorkerLoopGame::getGameStatus() {
    //despues ver como parsear todos los actores
    return GameNotification::getStatusMatchNotification(hordes, towers);
}

bool WorkerLoopGame::isTimeToCreateHorde() {
    time_t now;
    time(&now);

    return (now - timeLastHordeCreation) > timeBetweenHordeCreation;
}

void WorkerLoopGame::setTimeCreationHorde() {
    time_t now;
    time(&now);

    timeLastHordeCreation = now;
}

void WorkerLoopGame::createHordeAndNotify() {
    setTimeCreationHorde();

    time_t now;
    time(&now);

    unsigned hordeIndex = now % hordeType.size();
    int nextHordeType = hordeType.at(hordeIndex);

    unsigned pathIndex = now % paths.size();
    std::vector<Point> path = static_cast<std::vector<Point> &&>(paths.at(
            pathIndex));

    Horde *h = Horde::createHorde(nextHordeType, 3, path);

    hordes.insert(std::make_pair(hordeId, h));

    std::string statusGame =
            GameNotification::getNewHordeNotification(hordeId, nextHordeType,
                                                      3);

    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }

    ++hordeId;
}

void WorkerLoopGame::putTower(GameAction *pAction) {
    int type = pAction->typeOfTower;

    ActorTower *tower = nullptr;

    int towerId = towers.size();

    switch (type) {
        case TOWER_FIRE: {
            tower = new ActorTowerFire(towerId);
            break;
        }
        case TOWER_WATER: {
            tower = new ActorTowerWater(towerId);
            break;
        }
        case TOWER_AIR: {
            tower = new ActorTowerAir(towerId);
            break;
        }
        case TOWER_EARTH: {
            tower = new ActorTowerEarth(towerId);
            break;
        }
    }

    int actualX = pAction->x * CARTESIAN_TILE_WIDTH;
    int actualY = pAction->y * CARTESIAN_TILE_HEIGHT;
    tower->setPosition(pAction->x, pAction->y);
    towers.push_back(tower);

    std::string statusGame =
            GameNotification::getPutTowerNotification(towerId,
                                                      type,
                                                      actualX,
                                                      actualY);

    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }
}

void WorkerLoopGame::notifyMatchLoose() {
    std::string statusGame =
            MessageFactory::getMatchEndedNotification(GAME_STATUS_LOOSE);
    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }
}


/* ESTA ES LÓGICA DE NEGOCIO, Y VA EN EL SERVER...
     * bool towerIsShooting =
            enemy->itIsAlive() &&
            enemy->getCollisionCircle().hasCollisionWith(tower.getCollisionCircle());
    if (towerIsShooting) {
        tower.setIsShooting(true);
        int shotDamage = tower.getShotDamage();
        enemy->quitLifePoints(shotDamage);
        tower.sumExperiencePoints(shotDamage);
        std::cout << "la torre tiene " << tower.getExperiencePoints() << " de experiencia."
                  << std::endl;
    } else {
        tower.setIsShooting(false);
    }

    if (enemy->itIsAlive()) {
        enemy->setDirection(dir);
        enemy->moveTo(point.x, point.y);
    } else if (towerIsShooting) {
        tower.sumExperiencePoints(enemy->getBonus());
        std::cout << "la torre sumó bonus de " << enemy->getBonus() << " puntos de experiencia."
                  << std::endl;

        std::vector<Enemy*> enemies = static_cast<std::vector<Enemy *> &&>(horde->getEnemies());
        for (Enemy *enemy1 : enemies) {
            if (enemy1->itIsAlive()) {
                gameWon = false;
                break;
            } else {
                gameWon = true;
            }
        }
    }

    if (enemy->itIsAlive()) {
        if (point.x >= finalPoint.x && point.y >= finalPoint.y) {
            gameLoose = true;
        }
    }

    std::cout << "la torre tiene " << tower.getExperiencePoints() << " de experiencia." << std::endl;
     */
