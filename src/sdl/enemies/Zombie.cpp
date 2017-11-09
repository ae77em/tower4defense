#include "Zombie.h"

Zombie::Zombie(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 20;
    isAir = false;
    velocity = 10;
}
