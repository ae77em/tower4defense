#ifndef TP4_TOWERDEFENSE_LOOPGAME_H
#define TP4_TOWERDEFENSE_LOOPGAME_H

#include "../common/Thread.h"
#include "ServerPlayer.h"
#include "GameActor.h"
#include "GameAction.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "game-actors/enemies/Horde.h"
#include "game-actors/towers/ActorTower.h"
#include "../common/model/Map.h"
#include "game-actions/GameActionPutTower.h"
#include "game-actions/GameActionGetTowerInfo.h"
#include <map>
#include <string>
#include <list>
#include <vector>

static const std::string STR_PUT_TOWER = "put-tower";
static const std::string STR_GET_TOWER_INFO = "get-tower-info";
static const std::string STR_UPGRADE_TOWER = "upgrade-tower";

class WorkerLoopGame : public Thread{
private:
    std::map<int,ServerPlayer*>& players;
    std::list<GameAction*>& actions;
    std::mutex& mutexActions;
    model::Map map;

    std::map<int, Horde*> hordes;
    std::vector<ActorTower*> towers;

    int timeLastHordeCreation, hordeId;

    void run();

    std::string getGameStatus();

    bool isTimeToCreateHorde();

    void createHordeAndNotify();

    void putTower(GameActionPutTower *pAction);

    void notifyMatchLoose();

    bool actionsSuccessfullAttended(std::list<GameAction *> &actionsGame);

    void sendTowerInfo(GameActionGetTowerInfo *pInfo);

    void notifyMatchWin();

public:
    WorkerLoopGame(std::map<int,ServerPlayer*>& p,
                   std::list<GameAction*>& a,
                   std::mutex& m,
                   model::Map map);
};


#endif //TP4_TOWERDEFENSE_LOOPGAME_H
