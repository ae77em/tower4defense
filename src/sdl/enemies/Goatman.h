#ifndef TP4_TOWERDEFENSE_GOATMAN_H
#define TP4_TOWERDEFENSE_GOATMAN_H


#include "Enemy.h"

class Goatman : public Enemy {
public:
    Goatman(int x, int y, SDL_Renderer *renderer, LTexture *texture);

    void initializeSpritesData(int x, int y);
};


#endif //TP4_TOWERDEFENSE_GOATMAN_H
