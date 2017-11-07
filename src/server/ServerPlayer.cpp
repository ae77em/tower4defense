#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(ClientRequestHandler* crh, unsigned int aId)
        : clientRequestHandler(crh), id(aId),status(NOT_PLAYING){ }

unsigned int ServerPlayer::getId(){
    return id;
}

void ServerPlayer::sendData(std::string data){
    clientRequestHandler->sendData(data);
}

void ServerPlayer::launchRequesHandler(){
    clientRequestHandler->start();
}

void ServerPlayer::setGameId(std::string gId){
    gameId = gId;
}

void ServerPlayer::setStatus(unsigned int status){
    this->status = status;
}

unsigned int ServerPlayer::getStatus(){
    return this->status;
}

std::vector<std::string> & ServerPlayer::getElements() {
    return elements;
}

void ServerPlayer::setElements(std::vector<std::string> els) {
    elements = els;
}
