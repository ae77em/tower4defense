#include "BloodHawk.h"

BloodHawk::BloodHawk(int x, int y, SDL_Renderer *renderer, LTexture &t) : Enemy(x, y, renderer, t) {
    initialLifePoints = 100;
    velocity = 4;
    isAir = true;
}
