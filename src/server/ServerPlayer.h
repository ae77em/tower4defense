#ifndef TP4_TOWERDEFENSE_SERVERPLAYER_H
#define TP4_TOWERDEFENSE_SERVERPLAYER_H

enum MatchStatus {
    PLAYING = 1,
    NOT_PLAYING = 2,
    JOINED = 3
};



#include <iostream>

#include "ClientRequestHandler.h"

class ServerPlayer {

private:
    ClientRequestHandler *clientRequestHandler;
    unsigned int id;
    std::string gameId;
    unsigned int status;
    std::vector<std::string> elements;

public:
    std::vector<std::string> & getElements();

    void setElements(vector<string> elements);

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
