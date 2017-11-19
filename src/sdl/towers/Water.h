#ifndef TP4_TOWERDEFENSE_WATER_H
#define TP4_TOWERDEFENSE_WATER_H


#include "Tower.h"

class Water : public Tower {
public:
    Water(int x, int y, SDL_Renderer *renderer, Texture &t);
    ~Water();

    void initializeSpritesData(int x, int y);
};



#endif //TP4_TOWERDEFENSE_WATER_H
