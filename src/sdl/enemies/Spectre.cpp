#include "Spectre.h"

Spectre::Spectre(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 100;
    isAir = true;
    velocity = 6;
}
