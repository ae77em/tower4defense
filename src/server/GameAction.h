#ifndef TP4_TOWERDEFENSE_ACTIONGAME_H
#define TP4_TOWERDEFENSE_ACTIONGAME_H

#include <string>

class GameAction {
public:
    GameAction(const char a[10], int i, int i1, int i2);
    ~GameAction();

    std::string action;
    int x;
    int y;
    int typeOfTower;
};

#endif //TP4_TOWERDEFENSE_ACTIONGAME_H
