#ifndef TP4_TOWERDEFENSE_EARTH_H
#define TP4_TOWERDEFENSE_EARTH_H


#include "Tower.h"

class Earth : public Tower {
public:
    Earth(int x, int y, SDL_Renderer *renderer, Texture &t);
    ~Earth();

    void initializeSpritesData(int x, int y);
};


#endif //TP4_TOWERDEFENSE_EARTH_H
