#include "GameNotification.h"
#include "../common/Message.h"
#include "../common/Protocol.h"
#include "../common/MessageFactory.h"

#include <string>
#include <map>
#include <vector>

std::string GameNotification::getStatusMatchNotification(
        std::map<int, Horde *> hordes,
        std::vector<ActorTower *> towers) {
    Json::Value aTower(Json::objectValue);

    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_SCENARIO_STATUS;
    root["enemies"] = Json::arrayValue;

    for (auto it = hordes.begin(); it != hordes.end(); ++it) {
        Horde* horde = it->second;

        if (horde->shouldSendMoreData()){ // si la horda ya murió,
            // no mando más data
            for (auto g : horde->getEnemies()) {
                Json::Value aEnemy(Json::objectValue);

                aEnemy[HORDE_ID_KEY] = it->first;
                aEnemy[ENEMY_ID_KEY] = g->getId();
                aEnemy[XCOORD_KEY] = g->getXPosition();
                aEnemy[YCOORD_KEY] = g->getYPosition();
                aEnemy["direction"] = g->getDirection();
                aEnemy[ENERGY_PERCENTAJE_KEY] = g->getRemainingEnergyPercentaje();
                aEnemy[IS_ALIVE_KEY] = g->getIsAlive();

                root["enemies"].append(aEnemy);
            }
        }
    }

    root["towers"] = Json::arrayValue;

    ActorTower *currentTower = nullptr;
    for (unsigned i = 0; i < towers.size(); ++i) {
        currentTower = towers[i];

        aTower[TOWER_ID_KEY] = i;
        aTower["isShooting"] = currentTower->itIsShooting();

        root["towers"].append(aTower);
    }


    message.setData(root);

    return message.serialize();
}

std::string
GameNotification::getNewHordeNotification(int id,
                                        int hordeType,
                                        int amount) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_CREATE_HORDE;
    root[HORDE_ID_KEY] = id;
    root["hordeType"] = hordeType;
    root["amount"] = amount;

    message.setData(root);

    return message.serialize();
}

std::string
GameNotification::getPutTowerNotification(int id, int towerType, int x, int y) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_PUT_TOWER;
    root["towerId"] = id;
    root["towerType"] = towerType;
    root[XCOORD_KEY] = x;
    root[YCOORD_KEY] = y;

    message.setData(root);
    toReturn = message.serialize();

    return toReturn;
}

std::string
GameNotification::getUpgradeNotification(bool success, std::string info) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_APPLY_UPGRADE;
    root["success"] = success;
    root["info"] = info;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string
GameNotification::getTowerInfoNotification(int towerId,
                                           std::string experiencePointsInfo,
                                           std::string damageInfo,
                                           std::string rangeInfo,
                                           std::string reachInfo,
                                           std::string slowDownInfo) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_TOWER_INFO;
    root[TOWER_ID_KEY] = towerId;
    root["experience"] = experiencePointsInfo;
    root["damage"] = damageInfo;
    root["range"] = rangeInfo;
    root["reach"] = reachInfo;
    root["slowDown"] = slowDownInfo;

    message.setData(root);

    toReturn = message.serialize();

    return toReturn;
}

std::string GameNotification::getMatchEndedNotification(int matchStatus) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_MATCH_ENDED;
    root[MATCH_STATUS_KEY] = matchStatus;

    message.setData(root);

    return message.serialize();
}
