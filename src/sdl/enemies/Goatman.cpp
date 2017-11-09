#include "Goatman.h"

Goatman::Goatman(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 100;
    isAir = false;
    velocity = 2;
}
