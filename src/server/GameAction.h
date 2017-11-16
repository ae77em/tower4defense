#ifndef TP4_TOWERDEFENSE_ACTIONGAME_H
#define TP4_TOWERDEFENSE_ACTIONGAME_H

#include <string>

class GameAction {
public:
    GameAction(std::string a);
    ~GameAction();

    std::string action;
};

#endif //TP4_TOWERDEFENSE_ACTIONGAME_H
