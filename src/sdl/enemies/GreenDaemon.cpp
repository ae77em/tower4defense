#include "GreenDaemon.h"

GreenDaemon::GreenDaemon(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 300;
    velocity = 1;
    isAir = false;
}
