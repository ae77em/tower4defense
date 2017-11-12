#ifndef TP4_TOWERDEFENSE_LOOPGAME_H
#define TP4_TOWERDEFENSE_LOOPGAME_H


#include "../common/Thread.h"
#include "ServerPlayer.h"
#include "GameActor.h"
#include "GameAction.h"
#include "game-actors/enemies/ActorEnemy.h"

class WorkerLoopGame : public Thread{
private:
    std::map<int,ServerPlayer*>& players;
    std::list<GameAction*>& actions;
    std::mutex& mutexActions;
    std::vector<ActorEnemy*> gameActors;
    std::map<int,std::vector<ActorEnemy*>> hordas;
    bool& endSignal;

public:
    WorkerLoopGame(std::map<int,ServerPlayer*>& p,
                   std::list<GameAction*>& a,
                   std::mutex& m,
                    bool& endSignal);

    void run();

    void buildGameContext();

    string getGameStatus();
};


#endif //TP4_TOWERDEFENSE_LOOPGAME_H
