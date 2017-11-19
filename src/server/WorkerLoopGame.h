#ifndef TP4_TOWERDEFENSE_LOOPGAME_H
#define TP4_TOWERDEFENSE_LOOPGAME_H

#include "../common/Thread.h"
#include "ServerPlayer.h"
#include "GameActor.h"
#include "GameAction.h"
#include "game-actors/enemies/ActorEnemy.h"
#include "game-actors/enemies/Horde.h"
#include "game-actors/towers/ActorTower.h"
#include "../common/modelo/Mapa.h"
#include <map>
#include <string>
#include <list>
#include <vector>

class WorkerLoopGame : public Thread{
private:
    std::map<int,ServerPlayer*>& players;
    std::list<GameAction*>& actions;
    std::mutex& mutexActions;
    model::Mapa& map;

    std::map<int, Horde*> hordes;
    std::vector<ActorTower*> towers;

    int timeBetweenHordeCreation, timeLastHordeCreation, hordeId;
    std::vector<int> hordeType;
    std::vector<std::vector<Point>> paths;

public:
    WorkerLoopGame(std::map<int,ServerPlayer*>& p,
                   std::list<GameAction*>& a,
                   std::mutex& m,
                   model::Mapa& map);

    void run();

    void buildGameContext();

    std::string getGameStatus();

    bool isTimeToCreateHorde();

    void createHordeAndNotify();

    void setTimeCreationHorde();
};


#endif //TP4_TOWERDEFENSE_LOOPGAME_H
