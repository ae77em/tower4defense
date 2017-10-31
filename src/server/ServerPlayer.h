#ifndef TP4_TOWERDEFENSE_SERVERPLAYER_H
#define TP4_TOWERDEFENSE_SERVERPLAYER_H

#include "ClientRequestHandler.h"

class ClientRequestHandler;

class ServerPlayer {

private:
    ClientRequestHandler* clientRequestHandler;
    unsigned int id;
    unsigned int gameId;

public:
    explicit ServerPlayer(ClientRequestHandler* crh, unsigned int aId);
    unsigned int getId();

    void sendData(std::string data);
    void launchRequesHandler();

    void setGameId(int gameId);

};
#endif //TP4_TOWERDEFENSE_SERVERPLAYER_H
