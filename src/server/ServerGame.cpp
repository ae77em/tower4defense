//
// Created by federico on 29/10/17.
//

#include "ServerGame.h"

ServerGame::ServerGame(){
}

void ServerGame::addPlayer(ServerPlayer* sp){
    players.push_back(sp);
}

bool ServerGame::isTherePlace(){
    return players.size() < MAX_PLAYERS;
}

bool ServerGame::isFull(){
    return players.size() == MAX_PLAYERS;
}
