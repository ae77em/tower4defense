#include "Abmonible.h"

Abmonible::Abmonible(int x, int y, SDL_Renderer *renderer, Texture *texture)
        : Enemy(x, y, renderer, texture) {
    separationBetweenSprites = 1;
}
