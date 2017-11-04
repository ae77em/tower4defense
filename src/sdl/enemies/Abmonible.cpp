#include "Abmonible.h"

Abmonible::Abmonible(int x, int y, SDL_Renderer *renderer, LTexture &t) : Enemy(x, y, renderer, t) {
    lifePoints = 200;
    isAir = false;
    velocity = 1;
}
