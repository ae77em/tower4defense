#ifndef TP4_TOWERDEFENSE_GAMENOTIFICATION_H
#define TP4_TOWERDEFENSE_GAMENOTIFICATION_H

#include <string>
#include "game-actors/enemies/Horde.h"
#include "game-actors/towers/ActorTower.h"
#include <map>
#include <vector>

class GameNotification {
public:
    static std::string getStatusMatchNotification(std::map<int, Horde *> hordes,
                                              std::vector<ActorTower *> towers);

    static std::string getNewHordeNotification(int id,
                                               int hordeType,
                                               int amount);

    static std::string getPutTowerNotification(int id,
                                               int towerType,
                                               int x,
                                               int y);
};


#endif //TP4_TOWERDEFENSE_GAMENOTIFICATION_H
