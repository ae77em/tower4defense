#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(ClientRequestHandler* crh, unsigned int aId)
        : clientRequestHandler(crh), id(aId), status(NOT_PLAYING){ }

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

std::list<std::string> & ServerPlayer::getElements() {
    return elements;
}

void ServerPlayer::setElements(std::list<std::string> els) {
    elements = els;
}

// si esta jugando no notifico disponibilidad de elementos libres, o queda asi, nose
void ServerPlayer::kill() {
    std::cout << "GameServer: Esperando que termine el reciever del cliennte "<< id << std::endl;

    clientRequestHandler->join();

    std::cout << "GameServer: Termino el reciever del cliennte "<< id << std::endl;

    delete clientRequestHandler;
}
