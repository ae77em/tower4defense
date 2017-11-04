#include "Zombie.h"

Zombie::Zombie(int x, int y, SDL_Renderer *renderer, LTexture &t) : Enemy(x, y, renderer, t) {
    lifePoints = 20;
    isAir = false;
    velocity = 10;
}
