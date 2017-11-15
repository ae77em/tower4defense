#include "Abmonible.h"

Abmonible::Abmonible(int x, int y, SDL_Renderer *renderer, Texture *texture)
        : Enemy(model::Enemy(200, 1, "abominable"), x, y, renderer, texture) {
    separationBetweenSprites = 1;
}
