#include "GameLoopWorker.h"
#include "Point.h"
#include "ActorEnemy.h"
#include "ActorHorde.h"
#include "GameNotificationFactory.h"
#include "Map.h"
#include "Protocol.h"
#include "ActorTowerFire.h"
#include "ActorTowerWater.h"
#include "ActorTowerAir.h"
#include "ActorTowerEarth.h"
#include "GameActionPutTower.h"
#include "GameActionGetTowerInfo.h"
#include "GameActionUpgradeTower.h"
#include "Constants.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <algorithm>

GameLoopWorker::GameLoopWorker(std::map<int, ServerPlayer *> &p,
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

void GameLoopWorker::run() {
    std::cout
            << "GameLoopWorker: Hilo donde existe la partida arrancando"
            << std::endl;

    std::list<GameAction *> actionsGame;
    std::vector<ActorEnemy *> enemies;
    std::map<int, ActorHorde *>::iterator hordeIt;

    bool gameFinish = false;
    bool areEnemiesAlive;
    bool hordeIsAlive;
    std::string cause = "";
    ActorHorde *aHorde = nullptr;

    while (!gameFinish) {
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

                // Eliminar enemigos muertos
                // remove-erase idiom: remove mueve todos los elementos
                // a eliminar al final, y erase los elimina
                auto it = std::remove_if(enemies.begin(), enemies.end(),
                        [](ActorEnemy *x) { return not x->itIsAlive(); });
                enemies.erase(it, enemies.end());

                for (auto enemy : enemies) {
                    if (enemy->itIsAlive()) {
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
                    if (enemy->itIsAlive()) {
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

        /* Aproximadamente 22 fps. Lo dejamos así para darle tiempo a los
         * request de los usuarios a impactarse de forma relativamente
         * instantánea. */
        std::this_thread::sleep_for(std::chrono::milliseconds(45));
    }
}

/* Atender los comandos enviados por los clientes */
bool GameLoopWorker::actionsSuccessfullAttended(
        std::list<GameAction *> &actionsGame) {
    bool actionSuccessful = true;
    mutexActions.lock();

    std::string actionName;
    for (GameAction *action : actions) {
        std::cout << action->getName() << "  " << std::endl;

        actionName = action->getName();

        if (actionName == "game-explotion") {
            std::cout
                    << "GameLoopWorker: salgo porque explote "
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
    }

    actions.clear();

    mutexActions.unlock();

    return actionSuccessful;
}

std::string GameLoopWorker::getGameStatus() {
    return GameNotificationFactory::getStatusMatchNotification(hordes, towers);
}

bool GameLoopWorker::isTimeToCreateHorde() {
    time_t now;
    time(&now);

    /* Get delay from previus horde, or hardcoded 5 for the first horde */
    int delay = (hordeId == 0) ? 5 : std::get<3>(map.getHordes()[hordeId - 1]);
    return (now - timeLastHordeCreation) > delay;
}

void GameLoopWorker::createHordeAndNotify() {
    /* Mapa estatico que asocia nombres de enemigos a enums
     *
     * Los enums vienen de Protocol.h. El mapa es estatico para evitar
     * reconstruirlo cada vez que se llama a createHordeAndNotify.
     */
    static const std::map<std::string, int> enemy_str_to_type = {
        { "abominable", ENEMY_ABMONIBLE },
        { "bloodhawk", ENEMY_BLOOD_HAWK },
        { "goatman", ENEMY_GOATMAN },
        { "greendaemon", ENEMY_GREEN_DAEMON },
        { "spectre", ENEMY_SPECTRE },
        { "zombie", ENEMY_ZOMBIE },
    };

    /* Actualizar el momento de creacion de ultima horda */
    time_t now;
    time(&now);
    timeLastHordeCreation = now;

    /* Obtener los datos de la horda */
    const auto& horde = map.getHordes()[hordeId];
    const int horde_type = enemy_str_to_type.at(std::get<0>(horde));
    const int horde_size = std::get<1>(horde);
    const int path_index = std::get<2>(horde);

    /* Agregar nueva horda al juego */
    ActorHorde *h = ActorHorde::createHorde(horde_type, horde_size,
                                  map.getPaths()[path_index]);
    hordes.insert(std::make_pair(hordeId, h));

    /* Notificar a los clientes sobre la nueva horda */
    std::string statusGame = GameNotificationFactory::getNewHordeNotification(
                    hordeId, horde_type, horde_size);
    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }

    ++hordeId;
}

void GameLoopWorker::putTower(GameActionPutTower *pAction) {
    int type = pAction->getTypeOfTower();

    ActorTower *tower = nullptr;

    unsigned towerId = towers.size();

    switch (type) {
        case TOWER_FIRE: {
            tower = new ActorTowerFire();
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

    tower->setId(towerId);

    int actualX = pAction->getX() * CARTESIAN_TILE_WIDTH;
    int actualY = pAction->getY() * CARTESIAN_TILE_HEIGHT;
    tower->setPosition(actualX, actualY);
    towers.push_back(tower);

    std::string statusGame =
            GameNotificationFactory::getPutTowerNotification(towerId,
                                                      type,
                                                      actualX,
                                                      actualY);

    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }
}

void GameLoopWorker::notifyMatchWin() {
    std::string statusGame =
            GameNotificationFactory::getMatchEndedNotification(GAME_STATUS_WON);
    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }
}


void GameLoopWorker::notifyMatchLoose() {
    std::string statusGame =
            GameNotificationFactory::getMatchEndedNotification(GAME_STATUS_LOST);
    for (auto it = players.begin(); it != players.end(); ++it) {
        it->second->sendData(statusGame);
    }
}

void GameLoopWorker::sendTowerInfo(GameActionGetTowerInfo *pInfo) {
    int towerId = pInfo->getTowerId();
    int clientId = pInfo->getClientId();

    ActorTower *tower = towers.at(towerId);

    std::string data =
            GameNotificationFactory::getTowerInfoNotification(towerId,
                                       tower->getClassInfo(),
                                       tower->getExperiencePointsInfo(),
                                       tower->getShotDamageInfo(),
                                       tower->getRangeInfo(),
                                       tower->getReachInfo(),
                                       tower->getSlowDownPercentajeInfo());

    players.at(clientId)->sendData(data);
}

bool GameLoopWorker::allHordesWereCreatedYet() {
    return hordes.size() == map.getHordes().size();
}

void GameLoopWorker::upgradeTower(GameActionUpgradeTower *pInfo) {
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
            upgradeSuccessful = tower->upgradeRange();
            if (!upgradeSuccessful){
                info = "La torre no tiene los puntos necesarios para el "
                        "upgrade.";
            }

            break;
        }
        case UPGRADE_SLOWDOWN:{
            upgradeSuccessful = tower->upgradeSlowdown();
            if (!upgradeSuccessful){
                info = "La torre no tiene los puntos necesarios para el "
                        "upgrade.";
            }
            break;
        }
        case UPGRADE_REACH:{
            upgradeSuccessful = tower->upgradeReach();
            if (!upgradeSuccessful){
                info = "La torre no tiene los puntos necesarios para el "
                        "upgrade.";
            }
            break;
        }
        default:{
            upgradeSuccessful = false;
            info = "Tipo de upgrade no reconocido.";
        }
    }

    std::string data =
            GameNotificationFactory::getUpgradeNotification(upgradeSuccessful, info);

    players.at(clientId)->sendData(data);
}

