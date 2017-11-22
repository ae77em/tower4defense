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
#include "game-actions/GameActionPutTower.h"
#include "game-actions/GameActionGetTowerInfo.h"

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
                               model::Map mp) :
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

        /* Aproximadamente 30 fps. Lo dejamos así para darle tiempo a los
         * request de los usuarios a impactarse de forma relativamente
         * instantánea. */
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}

bool WorkerLoopGame::actionsSuccessfullAttended(std::list<GameAction *> &actionsGame) {
    bool actionSuccessful = true;
    mutexActions.lock();
    //GRONCHADA PARA REFACTORIZAR
    //IMPLEMENTAR CLASE QUE HAGA EL CIERRE DEL GAME LOOP

    /* Estos son los requests enviados por el cliente, por ejemplo,
     * poner torre... */
    std::string actionName;
    for (GameAction *action : actions) {
        std::cout << action->getName() << "  " << std::endl;

        actionName = action->getName();

        if (actionName == "game-explotion") {
            std::cout
                    << "WorkerLoopGame: salgo porque explote "
                    << std::endl;
            actionSuccessful = false;
            break;
        } else if (actionName == STR_PUT_TOWER) {
            putTower(dynamic_cast<GameActionPutTower *>(action));
        } else if (actionName == STR_GET_TOWER_INFO) {
            sendTowerInfo(dynamic_cast<GameActionGetTowerInfo *>(action));
        } else if (actionName == STR_UPGRADE_TOWER) {
            // get upgrade tower
        }
        actionsGame.push_back(action);
    }

    actions.clear();

    mutexActions.unlock();

    return actionSuccessful;
}

void WorkerLoopGame::buildGameContext() {
    /* HAY QUE LEVANTAR LAS HORDAS DEL ARCHIVO */
    timeLastHordeCreation = 0;

    hordeType.push_back((int) ENEMY_ZOMBIE);
    hordeType.push_back((int) ENEMY_GOATMAN);
    hordeType.push_back((int) ENEMY_SPECTRE);

    hordeId = 0;
}

std::string WorkerLoopGame::getGameStatus() {
    //despues ver como parsear todos los actores
    return GameNotification::getStatusMatchNotification(hordes, towers);
}

bool WorkerLoopGame::isTimeToCreateHorde() {
    time_t now;
    time(&now);

    return (now - timeLastHordeCreation) > map.getDelay();
}

void WorkerLoopGame::createHordeAndNotify() {
    /* HAY QUE LEVANTAR LAS HORDAS DEL ARCHIVO */

    time_t now;
    time(&now);

    /* Actualizar el momento de creacion de ultima horda */
    timeLastHordeCreation = now;

    unsigned hordeIndex = now % hordeType.size();
    int nextHordeType = hordeType.at(hordeIndex);

    unsigned pathIndex = now % map.getPaths().size();
    std::vector<Point> path = static_cast<std::vector<Point> &&>(
            map.getPaths().at(pathIndex));

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

void WorkerLoopGame::putTower(GameActionPutTower *pAction) {
    int type = pAction->getTypeOfTower();

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

    int actualX = pAction->getX() * CARTESIAN_TILE_WIDTH;
    int actualY = pAction->getY() * CARTESIAN_TILE_HEIGHT;
    tower->setPosition(actualX, actualY);
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

void WorkerLoopGame::sendTowerInfo(GameActionGetTowerInfo *pInfo) {
    int towerId = pInfo->getTowerId();
    int clientId = pInfo->getClientId();

    ActorTower tower = *towers.at(towerId);

    std::string data = MessageFactory::getTowerInfoNotification(towerId,
                                             tower.getShotDamageInfo(),
                                             tower.getRangeInfo(),
                                             tower.getReachInfo(),
                                             tower.getSlowDownPercentajeInfo());

    players.at(clientId)->sendData(data);
}
