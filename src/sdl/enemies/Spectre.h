#ifndef TP4_TOWERDEFENSE_SPRECTRE_H
#define TP4_TOWERDEFENSE_SPRECTRE_H


#include "Enemy.h"

class Spectre : public Enemy {
private:
    const int WALK_SPRITE_WIDTH = 105;
    const int WALK_SPRITE_HEIGHT = 119;

    const int DEATH_SPRITE_WIDTH = 200;
    const int DEATH_SPRITE_HEIGHT = 155;
public:
    Spectre(int x, int y, SDL_Renderer *renderer, LTexture *texture);

};


#endif //TP4_TOWERDEFENSE_SPRECTRE_H
