#ifndef TP4_TOWERDEFENSE_LISTENERLOOPGAME_H
#define TP4_TOWERDEFENSE_LISTENERLOOPGAME_H

#include <mutex>
#include "../common/Thread.h"
#include "../common/Message.h"
#include "../common/ThreadedQueue.h"
#include "GameAction.h"
#include <list>

class ListenerLoopGame : public Thread{
private:
    std::list<GameAction*>& actions;
    std::mutex& mutexActions;
    ThreadedQueue<Message>& queueMessagesGame;
    bool& endSignal;

public:
    ListenerLoopGame(std::list<GameAction*>& a,
                     std::mutex& m,
                     ThreadedQueue<Message>& q,
                        bool& endSignal);


    void run();
};


#endif //TP4_TOWERDEFENSE_LISTENERLOOPGAME_H
