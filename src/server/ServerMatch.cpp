#include "ServerMatch.h"
#include "Protocol.h"
#include "MessageFactory.h"
#include "Map.h"
#include "GameActionPutTower.h"
#include "GameActionUpgradeTower.h"

#include <algorithm>
#include <utility>
#include <list>
#include <string>

ServerMatch::ServerMatch(std::mutex &m, model::Map aMap):
                                      mutexPlayers(m),
                                      map(std::move(aMap)),
                                      endSignal(false),
                                      workerLoopGame(players,
                                                     actions,
                                                     mutexActionsGame,
                                      map)
                                      {
    elements.push_back(STR_WATER);
    elements.push_back(STR_AIR);
    elements.push_back(STR_FIRE);
    elements.push_back(STR_EARTH);
}

void ServerMatch::addPlayer(ServerPlayer* sp){
    players.insert(
            std::pair<int, ServerPlayer *>(sp->getId(),sp));
}

bool ServerMatch::elementsAreAvailables(std::list<std::string> elements) {
    std::list<std::string> othersElements;
    for (auto it=players.begin(); it!=players.end(); ++it){
        othersElements = it->second->getElements();

        for (std::string oe : othersElements) {
            if (std::find(elements.begin(), elements.end(), oe)
               != elements.end()) {
                return false;
            }
        }
    }
    return true;
}

bool ServerMatch::isElementsAvailables(){
    return ( getUnavailableElements().size() < MAX_ELEMENTS );
}

void ServerMatch::startGame() {
    changeStatusPlayesOnGame(PLAYING);

    workerLoopGame.start();
}

void ServerMatch::changeStatusPlayesOnGame(int status) {
    for (auto it=players.begin(); it!=players.end(); ++it){
        it->second->setStatus(status);
    }
}

void ServerMatch::addEventMessage(Message m){
}

void ServerMatch::notifyAll(std::string message) {
    for (auto it=players.begin(); it!=players.end(); ++it){
        it->second->sendData(message);
    }
}

void ServerMatch::notifyTo(int clientId, std::string message) {
    ServerPlayer *player = players.at(clientId);
    player->sendData(message);
}

bool ServerMatch::isPlaying() const {
    return playing;
}

void ServerMatch::setPlaying(bool playing) {
    ServerMatch::playing = playing;
}

void ServerMatch::removeElement(std::string elementName) {
    elements.remove(elementName);
}

std::list<std::string> ServerMatch::getElements() {
    return elements;
}

std::list<std::string> ServerMatch::getUnavailableElements() {
    std::list<std::string> toReturn;
    std::list<std::string> playerElements;

    for (auto it=players.begin(); it!=players.end(); ++it){
        playerElements = it->second->getElements();
        for (std::string el : playerElements){
            toReturn.push_back(el);
        }
    }
    return toReturn;
}

void ServerMatch::enableElements(int idPlayer) {
    ServerPlayer* sp = players.at(idPlayer);

    std::list<std::string>& elementsRecovered = sp->getElements();

    this->elements.merge(elementsRecovered);
}

void ServerMatch::removePlayer(int i) {
    players.erase(i);
}


int ServerMatch::getAmountPlayers() {
    return players.size();
}

void ServerMatch::kill() {
    mutexActionsGame.lock();
    actions.push_back(new GameAction("game-explotion"));
    mutexActionsGame.unlock();



    workerLoopGame.join();
}

void ServerMatch::markTile(int x, int y){
    std::string markTileStr = MessageFactory::getMarkTileNotification(x, y);
    Message message;

    message.deserialize(markTileStr);

    mutexPlayers.lock();
    notifyAll(message.serialize());
    mutexPlayers.unlock();
}

void ServerMatch::putTower(int typeOfTower, int x, int y) {
    mutexActionsGame.lock();
    actions.push_back(new GameActionPutTower(STR_PUT_TOWER, typeOfTower, x, y));
    mutexActionsGame.unlock();
}

void ServerMatch::castSpell(int x, int y) {
    std::string req = MessageFactory::getCastSpellNotification(x, y);
    mutexPlayers.lock();
    notifyAll(req);
    mutexPlayers.unlock();
}

void ServerMatch::upgradeTower(int clientId, int towerId, int upgradeType) {
    mutexActionsGame.lock();
    actions.push_back(
            new GameActionUpgradeTower(STR_UPGRADE_TOWER,
                                       clientId,
                                       towerId,
                                       upgradeType));
    mutexActionsGame.unlock();
}

void ServerMatch::towerInfo(int clientId, int towerId) {
    mutexActionsGame.lock();
    actions.push_back(new GameActionGetTowerInfo(STR_GET_TOWER_INFO,
                                                 towerId,
                                                 clientId));
    mutexActionsGame.unlock();
}

std::string &ServerMatch::getMapName() {
    return map.getName();
}

void ServerMatch::setMapName(const std::string &mapName) { }

