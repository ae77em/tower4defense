#ifndef TP4_TOWERDEFENSE_ZOMBIE_H
#define TP4_TOWERDEFENSE_ZOMBIE_H


#include "Enemy.h"

class Zombie : Enemy {
public:
    Zombie(int x, int y, SDL_Renderer *renderer, LTexture &t);

};


#endif //TP4_TOWERDEFENSE_ZOMBIE_H
