#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(ClientRequestHandler* crh, unsigned int aId)
        : clientRequestHandler(crh), id(aId){ }

unsigned int ServerPlayer::getId(){
    return id;
}

void ServerPlayer::sendData(std::string data){
    clientRequestHandler->sendData(data);
}

void ServerPlayer::launchRequesHandler(){
    clientRequestHandler->start();
}

void ServerPlayer::setGameId(int gId){
    gameId = gId;
}
