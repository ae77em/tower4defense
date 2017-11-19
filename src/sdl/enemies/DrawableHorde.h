#ifndef TP4_TOWERDEFENSE_DrawableHorde_H
#define TP4_TOWERDEFENSE_DrawableHorde_H

#include "Enemy.h"
#include <vector>

class DrawableHorde {
private:
    std::vector<Enemy*> enemies;

public:
    DrawableHorde();
    virtual ~DrawableHorde();

    std::vector<Enemy *> &getEnemies();

    void addEnemy(Enemy *e);

    Enemy *&getEnemieAt(int i);
};


#endif //TP4_TOWERDEFENSE_DrawableHorde_H
