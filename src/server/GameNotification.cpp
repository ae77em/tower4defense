#include "GameNotification.h"
#include "../common/Message.h"
#include "../common/Protocol.h"
#include "../common/MessageFactory.h"

#include <string>
#include <map>
#include <vector>

std::string GameNotification::getStatusMatchNotification(std::map<int,
        Horde *> hordes,
        std::vector<ActorTower *> towers) {
    std::string toReturn;
    Json::Value root(Json::objectValue);
    Message message;

    root[OPERATION_KEY] = SERVER_NOTIFICATION_SCENARIO_STATUS;
    root["enemies"] = Json::arrayValue;

    for (auto it = hordes.begin(); it != hordes.end(); ++it) {
        Horde* vectorActor = it->second;

        for (auto g : vectorActor->getEnemies()) {
            Json::Value aEnemy(Json::objectValue);

            aEnemy[HORDE_ID_KEY] = it->first;
            aEnemy[ENEMY_ID_KEY] = g->getId();
            aEnemy[XCOORD_KEY] = g->getXPosition();
            aEnemy[YCOORD_KEY] = g->getYPosition();
            aEnemy["direction"] = g->getDirection();
            aEnemy["energy"] = g->getEnergy();

            root["enemies"].append(aEnemy);
        }
    }

    root["towers"] = Json::arrayValue;

    ActorTower *currentTower = nullptr;
    for (unsigned i = 0; i < towers.size(); ++i) {
        Json::Value aTower(Json::objectValue);

        currentTower = towers[i];

        aTower[TOWER_ID_KEY] = i;
        aTower["isShooting"] = currentTower->itIsShooting();

        root["towers"].append(aTower);
    }


    message.setData(root);

    return message.serialize();
}
