#ifndef TP4_TOWERDEFENSE_AIR_H
#define TP4_TOWERDEFENSE_AIR_H


#include "Tower.h"

class Air : Tower {
    Air(int x, int y, SDL_Renderer *renderer, LTexture &t);
    ~Air();

    void initializeSpritesData(int x, int y);
};


#endif //TP4_TOWERDEFENSE_AIR_H
