#ifndef TP4_TOWERDEFENSE_SERVERGAME_H
#define TP4_TOWERDEFENSE_SERVERGAME_H

#define MAX_PLAYERS 4

#include <iostream>
#include "ServerPlayer.h"
#include "WorkerLoopGame.h"
#include "ListenerLoopGame.h"
#include "ActionGame.h"

class ServerGame {
private:
    std::vector<ServerPlayer*> players;
    //lo usaria para bloquear un jugador
    std::mutex& mutex;

    //lo uso para bloquear la lista deacciones a aplicar sobre el juego
    std::mutex mutexActionsGame;
    ThreadedQueue<Message> queueMessagesGame;
    WorkerLoopGame workerLoopGame;
    ListenerLoopGame listenerLoopGame;

    std::list<ActionGame*> actions;

    bool playing;

    std::list<std::string> elements;

public:
    bool isPlaying() const;

    void setPlaying(bool playing);

public:
    ServerGame(std::mutex& mutex);
    bool elementsAreAvailables(vector<string> elements);
    void addPlayer(ServerPlayer* sp);

    bool isFull();

    void startGame();

    void changeStatusPlayesOnGame(int i);

    void addEventMessage(Message m);

    void notifyAll(string basic_string);

    std::list<std::string> getElements();

    void removeElement(string elementName);
};


#endif //TP4_TOWERDEFENSE_SERVERGAME_H
