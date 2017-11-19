#ifndef TP4_TOWERDEFENSE_FIRE_H
#define TP4_TOWERDEFENSE_FIRE_H


#include "Tower.h"

class Fire : public Tower {
public:
    Fire(int x, int y, SDL_Renderer *renderer, Texture &t);
    ~Fire();

    void initializeSpritesData(int x, int y);
};



#endif //TP4_TOWERDEFENSE_FIRE_H
