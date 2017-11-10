#include "Abmonible.h"

Abmonible::Abmonible(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 200;
    isAir = false;
    velocity = 1;

    separationBetweenSprites = 1;
}
