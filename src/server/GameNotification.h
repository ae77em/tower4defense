#ifndef TP4_TOWERDEFENSE_GAMENOTIFICATION_H
#define TP4_TOWERDEFENSE_GAMENOTIFICATION_H

#include <string>
#include "game-actors/enemies/Horde.h"
#include "game-actors/towers/ActorTower.h"

class GameNotification {

public:
    static std::string getStatusMatchNotification(std::map<int, Horde *> hordes,
                                                  std::vector<ActorTower *> towers);
};


#endif //TP4_TOWERDEFENSE_GAMENOTIFICATION_H
