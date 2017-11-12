#ifndef TP4_TOWERDEFENSE_SERVERGAME_H
#define TP4_TOWERDEFENSE_SERVERGAME_H

#define MAX_PLAYERS 4

#include <iostream>
#include "ServerPlayer.h"
#include "WorkerLoopGame.h"
#include "ListenerLoopGame.h"
#include "GameAction.h"

class ServerGame {
private:
    std::vector<ServerPlayer*> players;
    //lo usaria para bloquear un jugador
    std::mutex& mutexPlayers;

    //lo uso para bloquear la lista de acciones a aplicar sobre el juego
    std::mutex mutexActionsGame;
    ThreadedQueue<Message> queueMessagesGame;
    WorkerLoopGame workerLoopGame;
    ListenerLoopGame listenerLoopGame;

    std::list<GameAction*> actions;

    bool playing = false;

    std::list<std::string> elements;

public:
    ServerGame(std::mutex& mutex);

    bool elementsAreAvailables(vector<string> elements);

    void addPlayer(ServerPlayer* sp);

    bool isPlaying() const;

    void setPlaying(bool playing);

    bool isFull();

    void startGame();

    void changeStatusPlayesOnGame(int i);

    void addEventMessage(Message m);

    void notifyAll(string basic_string);

    std::list<std::string> getElements();

    void removeElement(string elementName);

    list<string> getUnavailableElements();

    void enableElements(int idPlayer);

    void removePlayer(int i);

    void markTile(int x, int y);
};


#endif //TP4_TOWERDEFENSE_SERVERGAME_H
