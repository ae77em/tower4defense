#include "ServerGame.h"
#include "../sdl/Constants.h"
#include "../common/Protocol.h"
#include "../common/MessageFactory.h"

#include <algorithm>

ServerGame::ServerGame(std::mutex& m):mutexPlayers(m),
                                      workerLoopGame(players,actions,mutexActionsGame),
                                      listenerLoopGame(actions,mutexActionsGame,queueMessagesGame) {
    elements.push_back(STR_WATER);
    elements.push_back(STR_AIR);
    elements.push_back(STR_FIRE);
    elements.push_back(STR_TERRAIN);
}

void ServerGame::addPlayer(ServerPlayer* sp){
    players.push_back(sp);
}

bool ServerGame::elementsAreAvailables(vector<string> elements) {
    std::vector<std::string> othersElements;
    for (ServerPlayer *p : players){
        othersElements = p->getElements();
        for (std::string oe : othersElements) {
            if(std::find(elements.begin(), elements.end(), oe) != elements.end()) {
                return false;
            }
        }
    }
    return true;
}

bool ServerGame::isFull(){
    return players.size() == MAX_PLAYERS;
}

void ServerGame::startGame() {
    changeStatusPlayesOnGame(PLAYING);

    workerLoopGame.buildGameContext();

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

std::list<std::string> ServerGame::getUnavailableElements() {
    std::list<std::string> toReturn;
    std::vector<std::string> playerElements;

    for(ServerPlayer* sp : players){
        playerElements = sp->getElements();
        for (std::string el : playerElements){
            toReturn.push_back(el);
        }
    }

    return toReturn;
}

void ServerGame::enableElements(int idPlayer) {
    //Cambiar a hash el contenedor de jugadores para remover por elementos ID
    //TOMAR ELEMENTOS PARA CARGARLOS A LA LISTA DE ELMENTOS

}

void ServerGame::removePlayer(int i) {
    //QUITAR JUGADOR DEL HASH
}

void ServerGame::markTile(int x, int y){
    std::string markTileStr = MessageFactory::getMarkTileNotification(x, y);
    Message message;

    message.deserialize(markTileStr);

    mutexPlayers.lock();
    notifyAll(message.serialize());
    mutexPlayers.unlock();
}
