#include "GreenDaemon.h"

GreenDaemon::GreenDaemon(int x, int y, SDL_Renderer *renderer, LTexture &t) : Enemy(x, y, renderer, t) {
    initialLifePoints = 300;
    velocity = 1;
    isAir = false;
}