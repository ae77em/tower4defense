#ifndef TP4_TOWERDEFENSE_LOOPGAME_H
#define TP4_TOWERDEFENSE_LOOPGAME_H

#include "Thread.h"
#include "ServerPlayer.h"
#include "GameActor.h"
#include "GameAction.h"
#include "ActorEnemy.h"
#include "ActorHorde.h"
#include "ActorTower.h"
#include "Map.h"
#include "GameActionPutTower.h"
#include "GameActionGetTowerInfo.h"
#include "GameActionUpgradeTower.h"
#include <map>
#include <string>
#include <list>
#include <vector>

static const std::string STR_PUT_TOWER = "put-tower";
static const std::string STR_GET_TOWER_INFO = "get-tower-info";
static const std::string STR_UPGRADE_TOWER = "upgrade-tower";

class GameLoopWorker : public Thread{
private:
    std::map<int,ServerPlayer*>& players;
    std::list<GameAction*>& actions;
    std::mutex& mutexActions;
    model::Map map;

    std::map<int, ActorHorde*> hordes;
    std::vector<ActorTower*> towers;

    int timeLastHordeCreation, hordeId;

    std::string getGameStatus();

    bool isTimeToCreateHorde();

    void createHordeAndNotify();

    void putTower(GameActionPutTower *pAction);

    void notifyMatchLoose();

    bool actionsSuccessfullAttended(std::list<GameAction *> &actionsGame);

    void sendTowerInfo(GameActionGetTowerInfo *pInfo);

    void notifyMatchWin();

    bool allHordesWereCreatedYet();

    void upgradeTower(GameActionUpgradeTower *pInfo);

public:
    GameLoopWorker(std::map<int,ServerPlayer*>& p,
                   std::list<GameAction*>& a,
                   std::mutex& m,
                   model::Map map);

    ~GameLoopWorker();

    void run();
};


#endif //TP4_TOWERDEFENSE_LOOPGAME_H
