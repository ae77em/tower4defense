#include "GameAction.h"

GameAction::GameAction(std::string n) : name(n) { }

GameAction::~GameAction() { }

const std::string &GameAction::getName() const {
    return name;
}
