#ifndef TP4_TOWERDEFENSE_GAMEACTIONGETTOWERINFO_H
#define TP4_TOWERDEFENSE_GAMEACTIONGETTOWERINFO_H

#include <string>
#include "GameAction.h"

class GameActionGetTowerInfo : public GameAction {
public:
    GameActionGetTowerInfo(std::string name, int towerId, int clientId);
    ~GameActionGetTowerInfo() override;

private:
    int towerId;
    int clientId;

public:
    int getTowerId() const;

    int getClientId() const;
};


#endif //TP4_TOWERDEFENSE_GAMEACTIONGETTOWERINFO_H
