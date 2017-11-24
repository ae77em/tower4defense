#ifndef TP4_TOWERDEFENSE_GAMEACTIONUPGRADETOWER_H
#define TP4_TOWERDEFENSE_GAMEACTIONUPGRADETOWER_H

#include <string>
#include "../GameAction.h"

class GameActionUpgradeTower : public GameAction {
public:
    GameActionUpgradeTower(const std::string name,
                           int clientId,
                           int towerId,
                           int upgradeType);
    ~GameActionUpgradeTower();

    int getTowerId() const;

    int getClientId() const;

    int getUpgradeType() const;

private:
    std::string name;
    int towerId;
    int clientId;
    int upgradeType;
};


#endif //TP4_TOWERDEFENSE_GAMEACTIONUPGRADETOWER_H
