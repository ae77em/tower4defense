#ifndef TP4_TOWERDEFENSE_ACTIONGAME_H
#define TP4_TOWERDEFENSE_ACTIONGAME_H

#include <string>

class GameAction {
public:
    GameAction(std::string n);
    virtual ~GameAction();

private:
    std::string name;
public:
    const std::string &getName() const;

    void setAction(const std::string &action);
};

#endif //TP4_TOWERDEFENSE_ACTIONGAME_H
