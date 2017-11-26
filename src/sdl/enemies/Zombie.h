#ifndef TP4_TOWERDEFENSE_ZOMBIE_H
#define TP4_TOWERDEFENSE_ZOMBIE_H

#include "Enemy.h"

class Zombie : public Enemy {
public:
    Zombie(int x, int y, SDL_Renderer *renderer, Texture *texture);

    void initializeSpritesData();
};


#endif //TP4_TOWERDEFENSE_ZOMBIE_H
