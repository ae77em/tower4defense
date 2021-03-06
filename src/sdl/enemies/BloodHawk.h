#ifndef TP4_TOWERDEFENSE_BLOODHAWK_H
#define TP4_TOWERDEFENSE_BLOODHAWK_H

#include "Enemy.h"

class BloodHawk : public Enemy {
public:
    BloodHawk(int x, int y, SDL_Renderer *renderer, Texture *texture);
    virtual ~BloodHawk();

    void initializeSpritesData();
};


#endif //TP4_TOWERDEFENSE_BLOODHAWK_H
