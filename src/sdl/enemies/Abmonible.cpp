#include "Abmonible.h"

Abmonible::Abmonible(int x, int y, SDL_Renderer *renderer) : Enemy(x, y, renderer) {
    initialLifePoints = 200;
    isAir = false;
    velocity = 1;
}
