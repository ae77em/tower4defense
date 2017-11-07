#ifndef TP4_TOWERDEFENSE_SERVERPLAYER_H
#define TP4_TOWERDEFENSE_SERVERPLAYER_H

#define PLAYING 1
#define NOT_PLAYING 2

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

public:
    explicit ServerPlayer(ClientRequestHandler *crh, unsigned int aId);

    unsigned int getId();


    void sendData(std::string data);

    void launchRequesHandler();

    void setGameId(std::string gameId);

    void setStatus(unsigned int status);

    unsigned int getStatus();
};

#endif //TP4_TOWERDEFENSE_SERVERPLAYER_H
