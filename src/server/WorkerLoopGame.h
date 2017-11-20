#ifndef TP4_TOWERDEFENSE_LOOPGAME_H
#define TP4_TOWERDEFENSE_LOOPGAME_H

static const char *const STR_PUT_TOWER = "put-tower";

static const char *const STR_GET_TOWER_INFO = "get-tower-info";

static const char *const STR_UPGRADE_TOWER = "upgrade-tower";

#include "../common/Thread.h"
#include "ServerPlayer.h"
#include "GameActor.h"
#include "GameAction.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "game-actors/enemies/Horde.h"
#include "game-actors/towers/ActorTower.h"
#include "../common/model/Map.h"
#include <map>
#include <string>
#include <list>
#include <vector>

class WorkerLoopGame : public Thread{
private:
    std::map<int,ServerPlayer*>& players;
    std::list<GameAction*>& actions;
    std::mutex& mutexActions;
    model::Map& map;

    std::map<int, Horde*> hordes;
    std::vector<ActorTower*> towers;

    int timeBetweenHordeCreation, timeLastHordeCreation, hordeId;
    std::vector<int> hordeType;
    std::vector<std::vector<Point>> paths;

public:
    WorkerLoopGame(std::map<int,ServerPlayer*>& p,
                   std::list<GameAction*>& a,
                   std::mutex& m,
                   model::Map& map);

    void run();

    void buildGameContext();

    std::string getGameStatus();

    bool isTimeToCreateHorde();

    void createHordeAndNotify();

    void setTimeCreationHorde();

    void putTower(GameAction *pAction);
};


#endif //TP4_TOWERDEFENSE_LOOPGAME_H
