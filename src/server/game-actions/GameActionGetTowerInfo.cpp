#include "GameActionGetTowerInfo.h"

GameActionGetTowerInfo::GameActionGetTowerInfo(std::string name,
                                               int tId,
                                               int cId)
: GameAction(name) {
    towerId = tId;
    clientId = cId;
}

GameActionGetTowerInfo::~GameActionGetTowerInfo() { }

int GameActionGetTowerInfo::getTowerId() const {
    return towerId;
}

int GameActionGetTowerInfo::getClientId() const {
    return clientId;
}


