#include "ServerGame.h"
#include "../sdl/Constants.h"
#include "../common/Protocol.h"

ServerGame::ServerGame(std::mutex& m):mutex(m),
                                      workerLoopGame(players,actions,mutexActionsGame),
                                      listenerLoopGame(actions,mutexActionsGame,queueMessagesGame) {
    elements.push_back(WATER);
    elements.push_back(AIR);
    elements.push_back(FIRE);
    elements.push_back(TERRAIN);
}

void ServerGame::addPlayer(ServerPlayer* sp){
    players.push_back(sp);
}

bool ServerGame::isElementAvailibity(std::string element){
    for(ServerPlayer* p : players)
        if( p->getElement() == element)
            return false;

    return true;
}

bool ServerGame::isFull(){
    return players.size() == MAX_PLAYERS;
}

void ServerGame::startGame() {
    changeStatusPlayesOnGame(PLAYING);

    workerLoopGame.buildContextGame();

    listenerLoopGame.start();
    workerLoopGame.start();
}

void ServerGame::changeStatusPlayesOnGame(int status) {
    for(ServerPlayer* sp : players){
        sp->setStatus(status);
    }
}

void ServerGame::addEventMessage(Message m){
    queueMessagesGame.push(m);
}

void ServerGame::notifyAll(string message) {
    for(ServerPlayer* sp : players){
        sp->sendData(message);
    }
}

bool ServerGame::isPlaying() const {
    return playing;
}

void ServerGame::setPlaying(bool playing) {
    ServerGame::playing = playing;
}

void ServerGame::removeElement(std::string elementName) {
    elements.remove(elementName);
}

std::list<std::string> ServerGame::getElements() {
    return elements;
}
