//
// Created by federico on 29/10/17.
//

#ifndef TP4_TOWERDEFENSE_SERVERGAME_H
#define TP4_TOWERDEFENSE_SERVERGAME_H

#define MAX_PLAYERS 4

#include <iostream>
#include "ServerPlayer.h"

class ServerPlayer;

class ServerGame {
private:
    std::vector<ServerPlayer*> players;

public:
    ServerGame();
    bool isTherePlace();
    void addPlayer(ServerPlayer* sp);
};


#endif //TP4_TOWERDEFENSE_SERVERGAME_H
