//
// Created by federico on 28/10/17.
//

#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(ClientRequestHandler* crh,unsigned int aId):id(aId){
    this->clientRequestHandler = crh;
}

unsigned int ServerPlayer::getId(){
    return id;
}

void ServerPlayer::sendData(std::string data){
    this->clientRequestHandler->sendData(data);
}

void ServerPlayer::launchRequesHandler(){
    this->clientRequestHandler->start();
}
