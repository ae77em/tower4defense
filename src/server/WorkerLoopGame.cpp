#include "WorkerLoopGame.h"
#include "../common/Point.h"
#include "../sdl/Constants.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "game-actors/enemies/Horde.h"
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
#include "game-actions/GameActionUpgradeTower.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <map>
#include <string>
#include <list>
#include <vector>


#define MAX_HORDES 3

//HAY QUE AGREGAR MUTEX PLAYERD PARA QUE BLOQUEE LA LISTA DE SERVER PLAYERS
//PUEDE SER QUE SE ESTE REMOVIENDO ALGUNO EN OTRO HILO Y LUEGO EXPLOTE
WorkerLoopGame::WorkerLoopGame(std::map<int, ServerPlayer *> &p,
                               std::list<GameAction *> &a,
                               std::mutex &m,
                               model::Map mp) :
        players(p),
        actions(a),
        mutexActions(m),
        map(mp),
        hordes(),
        towers(),
        timeLastHordeCreation(0),
        hordeId(0) {}

void WorkerLoopGame::run() {
    std::cout
            << "WorkerLoopGame: Hilo donde existe la partida arrancando"
            << std::endl;

    std::list<GameAction *> actionsGame;
    std::vector<ActorEnemy *> enemies;
    std::map<int, Horde *>::iterator hordeIt;

    bool gameFinish = false;
    bool areEnemiesAlive;
    bool hordeIsAlive;
    std::string cause = "";
    Horde *aHorde = nullptr;

    while (!gameFinish) {
        /* Si debo crear una horda, la creo... */
        if (!allHordesWereCreatedYet()) {
            if (isTimeToCreateHorde()) {
                createHordeAndNotify();
            }
        }

        if (!actionsSuccessfullAttended(actionsGame)) {
            break;
        }

        /* Muevo los enemigos. */
        for (hordeIt = hordes.begin(); hordeIt != hordes.end(); ++hordeIt) {
            aHorde = hordeIt->second;
            if (aHorde->itIsAlive()) {
                enemies = aHorde->getEnemies();

                for (auto enemy : enemies) {
                    if (enemy->getIsAlive()) { // los enemigos vivos, viven...
                        enemy->advance();
                        if (enemy->hasEndedThePath()) {
                            gameFinish = true;
                            notifyMatchLoose();
                        }
                    }
                }
            }
        }

        /* Hago actuar las torres. */
        for (auto tower : towers) {
            if (tower->isReadyToShoot()) {
                for (hordeIt = hordes.begin();
                     hordeIt != hordes.end(); ++hordeIt) {
                    tower->attack(hordeIt->second);
                }
            }
        }

        /* Valido que haya bichos vivos. */
        areEnemiesAlive = false;
        for (hordeIt = hordes.begin(); hordeIt != hordes.end(); ++hordeIt) {
            aHorde = hordeIt->second;
            if (aHorde->itIsAlive()){
                enemies = aHorde->getEnemies();
                hordeIsAlive = false;

                for (auto enemy : enemies) {
                    if (enemy->getIsAlive()) {
                        areEnemiesAlive = true;
                        hordeIsAlive = true;
                        break;
                    }
                }
                if (!hordeIsAlive) {
                    aHorde->setIsAlive(false);
                }
            }
        }
        if (!areEnemiesAlive) {
            gameFinish = true;
            notifyMatchWin();
        }


        /* Notifico el estado del juego a todos los jugadores. */
        for (auto it = players.begin(); it != players.end(); ++it) {
            it->second->sendData(getGameStatus());
        }

        /* Limpio la lista para no ejecutar request viejos. */
        actionsGame.clear();

        /* Aproximadamente 30 fps. Lo dejamos así para darle tiempo a los
         * request de los usuarios a impactarse de forma relativamente
         * instantánea. */
        std::this_thread::sleep_for(std::chrono::milliseconds(45));
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
            upgradeTower(dynamic_cast<GameActionUpgradeTower*>(action));
        }
        //actionsGame.push_back(action);
    }

    actions.clear();

    mutexActions.unlock();

    return actionSuccessful;
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
    time_t now;
    time(&now);

    /* Actualizar el momento de creacion de ultima horda */
    timeLastHordeCreation = now;

    int enemyType = ENEMY_ABMONIBLE; // TODO: tomar el tipo del archivo

    /* Agregar nueva horda al juego */
    //FIXME: siempre crea el mismo tipo de horda
    int horde_size = map.getHordes()[hordeId].second.size();
    Horde *h = Horde::createHorde(enemyType, horde_size,
                              map.getPaths()[map.getHordes()[hordeId].first]);

    unsigned pathIndex = now % map.getPaths().size();

    std::vector<Point> path = map.getPaths().at(pathIndex);

    std::cout
            << "cantidad de paths disponibles: "
            << map.getPaths().size()
            << std::endl;
    std::cout
            << "cree horda con el path "
            << pathIndex
            << std::endl;
    std::cout
            << "path: ";
    for (Point point : path) {
        std::cout << point.toString();
        std::cout << " - ";
    }
    std::cout << std::endl;

    hordes.insert(std::make_pair(hordeId, h));

    /* Notificar a los clientes sobre la nueva horda */std::string statusGame =
            GameNotification::getNewHordeNotification(
                    hordeId, enemyType, horde_size);
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

void WorkerLoopGame::notifyMatchWin() {
    std::string statusGame =
            GameNotification::getMatchEndedNotification(GAME_STATUS_WON);
    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }
}


void WorkerLoopGame::notifyMatchLoose() {
    std::string statusGame =
            GameNotification::getMatchEndedNotification(GAME_STATUS_LOST);
    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }
}

void WorkerLoopGame::sendTowerInfo(GameActionGetTowerInfo *pInfo) {
    int towerId = pInfo->getTowerId();
    int clientId = pInfo->getClientId();

    ActorTower tower = *towers.at(towerId);

    std::string data =
            GameNotification::getTowerInfoNotification(towerId,
                                              tower.getExperiencePointsInfo(),
                                              tower.getShotDamageInfo(),
                                              tower.getRangeInfo(),
                                              tower.getReachInfo(),
                                              tower.getSlowDownPercentajeInfo());

    players.at(clientId)->sendData(data);
}

bool WorkerLoopGame::allHordesWereCreatedYet() {
    return hordes.size() == MAX_HORDES;
}

void WorkerLoopGame::upgradeTower(GameActionUpgradeTower *pInfo) {
    int clientId = pInfo->getClientId();
    int towerId = pInfo->getTowerId();
    int upgradeType = pInfo->getUpgradeType();
    bool upgradeSuccessful;
    std::string info = "Upgrade exitoso.";

    ActorTower *tower = towers.at(towerId);

    /* Hago el upgrade correspondiente. */
    switch (upgradeType){
        case UPGRADE_DAMAGE:{
            upgradeSuccessful = tower->upgradeDamage();
            if (!upgradeSuccessful){
                info = "La torre no tiene los puntos necesarios para el "
                        "upgrade.";
            }
            break;
        }
        case UPGRADE_RANGE:{
            //upgradeSuccessful = tower->upgradeRange();
            upgradeSuccessful = false;
            info = "No implementado.";
            break;
        }
        case UPGRADE_SLOWDOWN:{
            //upgradeSuccessful = tower->upgradeSlowdown();
            upgradeSuccessful = false;
            info = "No implementado.";
            break;
        }
        case UPGRADE_REACH:{
            //upgradeSuccessful = tower->upgradeReach();
            upgradeSuccessful = false;
            info = "No implementado.";
            break;
        }
        default:{
            upgradeSuccessful = false;
            info = "Tipo de upgrade no reconocido.";
        }
    }

    std::string data =
            GameNotification::getUpgradeNotification(upgradeSuccessful, info);

    players.at(clientId)->sendData(data);
}

