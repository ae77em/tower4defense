#ifndef TP4_TOWERDEFENSE_SERVERPLAYER_H
#define TP4_TOWERDEFENSE_SERVERPLAYER_H

#include <iostream>
#include <string>
#include <list>
#include "ClientRequestHandler.h"

enum PlayerMatchStatus {
    PLAYING = 1,
    NOT_PLAYING = 2,
    JOINED = 3
};

class ServerPlayer {
private:
    ClientRequestHandler *clientRequestHandler;
    unsigned int id;
    std::string gameId;
    unsigned int status;
    std::list<std::string> elements;

public:
    std::list<std::string> & getElements();

    void setElements(std::list<std::string> elements);

    explicit ServerPlayer(ClientRequestHandler *crh, unsigned int aId);

    unsigned int getId();

    void sendData(std::string data);

    void launchRequesHandler();

    void setGameId(std::string gameId);

    std::string getGameId(){
        return gameId;
    }

    void setStatus(unsigned int status);

    unsigned int getStatus();

    void kill();
};

#endif //TP4_TOWERDEFENSE_SERVERPLAYER_H
