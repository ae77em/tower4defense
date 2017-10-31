#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(ClientRequestHandler* crh, unsigned int aId):id(aId){
    this->clientRequestHandler = crh;
}

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
