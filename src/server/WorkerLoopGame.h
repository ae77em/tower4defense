#ifndef TP4_TOWERDEFENSE_LOOPGAME_H
#define TP4_TOWERDEFENSE_LOOPGAME_H


#include "../common/Thread.h"
#include "ServerPlayer.h"
#include "GameActor.h"
#include "ActionGame.h"

class WorkerLoopGame : public Thread{
private:
    std::vector<ServerPlayer*>& players;
    std::list<ActionGame*>& actions;
    std::mutex& mutexActions;
    std::vector<GameActor*> gameActors;

public:
    WorkerLoopGame(std::vector<ServerPlayer*>& p,
                   std::list<ActionGame*>& a,
                   std::mutex& m);

    void run();

    void buildContextGame();
};


#endif //TP4_TOWERDEFENSE_LOOPGAME_H