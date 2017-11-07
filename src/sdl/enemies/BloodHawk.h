#ifndef TP4_TOWERDEFENSE_BLOODHAWK_H
#define TP4_TOWERDEFENSE_BLOODHAWK_H


#include "Enemy.h"

class BloodHawk : public Enemy {
private:
    LTexture texture;

public:
    BloodHawk(int x, int y, SDL_Renderer *renderer);

    bool loadMedia();

};


#endif //TP4_TOWERDEFENSE_BLOODHAWK_H
