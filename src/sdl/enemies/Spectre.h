#ifndef TP4_TOWERDEFENSE_SPRECTRE_H
#define TP4_TOWERDEFENSE_SPRECTRE_H

#include "Enemy.h"

class Spectre : public Enemy {
public:
    Spectre(int x, int y, SDL_Renderer *renderer, Texture *texture);

    void initializeSpritesData();
};


#endif //TP4_TOWERDEFENSE_SPRECTRE_H
