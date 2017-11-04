#include "Abmonible.h"

Abmonible::Abmonible(int x, int y, SDL_Renderer *renderer, LTexture &t) : Enemy(x, y, renderer, t) {
    initialLifePoints = 200;
    isAir = false;
    velocity = 1;
}
