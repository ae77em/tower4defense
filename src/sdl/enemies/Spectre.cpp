#include "Spectre.h"

Spectre::Spectre(int x, int y, SDL_Renderer *renderer, LTexture &t) : Enemy(x, y, renderer) {
    initialLifePoints = 100;
    isAir = true;
    velocity = 6;
}
