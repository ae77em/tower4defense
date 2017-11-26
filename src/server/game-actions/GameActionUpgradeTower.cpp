#include "GameActionUpgradeTower.h"

#include <utility>
#include "GameAction.h"

GameActionUpgradeTower::GameActionUpgradeTower(const std::string name,
                                               int cId,
                                               int tId,
                                               int uType)
        : GameAction(std::move(name)) {
    towerId = tId;
    upgradeType = uType;
    clientId = cId;
}

GameActionUpgradeTower::~GameActionUpgradeTower() = default;


int GameActionUpgradeTower::getClientId() const {
    return clientId;
}

int GameActionUpgradeTower::getTowerId() const {
    return towerId;
}

int GameActionUpgradeTower::getUpgradeType() const {
    return upgradeType;
}

