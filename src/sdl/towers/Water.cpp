#include "Water.h"
#include "../Utils.h"

Water::Water(int x, int y, SDL_Renderer *renderer, Texture &t)
        : Tower(x, y, renderer, t) {
    initializeSpritesData(x,y);
}

Water::~Water(){}

void Water::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para iddle */
    idleStartX = 1;
    idleStartY = 17;
    idleSpriteWidth = 56;
    idleSpriteHeight = 116;
    numberOfIdleSprites = 13;

    idleBox.x = initialSreenPos.x;
    idleBox.y = initialSreenPos.y;
    idleBox.w = idleSpriteWidth;
    idleBox.h = idleSpriteHeight;

    /* datos sprites para disparar */
    shotStartX = 1;
    shotStartY = 209;
    shotSpriteWidth = 56;
    shotSpriteHeight = 116;
    numberOfShotSprites = 13;

    shotBox.x = initialSreenPos.x;
    shotBox.y = initialSreenPos.y;
    shotBox.w = shotSpriteWidth;
    shotBox.h = shotSpriteHeight;
}

