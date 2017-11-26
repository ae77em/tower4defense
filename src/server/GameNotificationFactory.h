#ifndef TP4_TOWERDEFENSE_GAMENOTIFICATION_H
#define TP4_TOWERDEFENSE_GAMENOTIFICATION_H

#include <string>
#include "Horde.h"
#include "ActorTower.h"
#include <map>
#include <vector>

class GameNotificationFactory {
public:
    static std::string getStatusMatchNotification(
            std::map<int, Horde *> &hordes,
            std::vector<ActorTower *> &towers);

    static std::string getNewHordeNotification(int id,
                                               int hordeType,
                                               int amount);

    static std::string getPutTowerNotification(int id,
                                               int towerType,
                                               int x,
                                               int y);

    static std::string getUpgradeNotification(bool success, std::string message);

    static std::string getTowerInfoNotification(int towerId,
                                                std::string classInfo,
                                                std::string experiencePointsInfo,
                                                std::string damageInfo,
                                                std::string rangeInfo,
                                                std::string reachInfo,
                                                std::string slowDownInfo);

    static std::string getMatchEndedNotification(int matchStatus);
};


#endif //TP4_TOWERDEFENSE_GAMENOTIFICATION_H
