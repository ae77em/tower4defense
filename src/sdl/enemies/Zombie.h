#ifndef TP4_TOWERDEFENSE_ZOMBIE_H
#define TP4_TOWERDEFENSE_ZOMBIE_H

#include "Enemy.h"

class Zombie : public Enemy {
private:
    const int WALK_SPRITE_WIDTH = 105;
    const int WALK_SPRITE_HEIGHT = 119;

    const int DEATH_SPRITE_WIDTH = 200;
    const int DEATH_SPRITE_HEIGHT = 155;
public:
    Zombie(int x, int y, SDL_Renderer *renderer, LTexture *texture);

    void initializeSpritesData(int x, int y);
};


#endif //TP4_TOWERDEFENSE_ZOMBIE_H
