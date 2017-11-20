#include "GameAction.h"

GameAction::GameAction(const char a[10], int ax, int ay, int type)
        : action(a), x(ax), y(ay), typeOfTower(type) { }
