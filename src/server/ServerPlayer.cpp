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

const string &ServerPlayer::getElement() const {
    return element;
}

void ServerPlayer::setElement(const string &element) {
    ServerPlayer::element = element;
}
