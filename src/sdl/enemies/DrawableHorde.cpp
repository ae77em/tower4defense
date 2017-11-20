#include "DrawableHorde.h"
#include "Abmonible.h"
#include "../../common/Protocol.h"
#include <vector>

DrawableHorde::DrawableHorde() {}

DrawableHorde::~DrawableHorde() {}

void DrawableHorde::addEnemy(Enemy *e){
    enemies.push_back(e);
}

std::vector<Enemy *> &DrawableHorde::getEnemies() {
    return enemies;
}

Enemy* &DrawableHorde::getEnemieAt(int i) {
    return enemies.at((unsigned)i);
}
