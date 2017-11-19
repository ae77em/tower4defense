#include "Fire.h"
#include "../Utils.h"

Fire::Fire(int x, int y, SDL_Renderer *renderer, Texture &t)
        : Tower(x, y, renderer, t) {
    initializeSpritesData(x,y);
}

Fire::~Fire(){}

void Fire::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para iddle */
    idleStartX = 1;
    idleStartY = 1;
    idleSpriteWidth = 62;
    idleSpriteHeight = 103;
    numberOfIdleSprites = 15;

    idleBox.x = initialSreenPos.x;
    idleBox.y = initialSreenPos.y;
    idleBox.w = idleSpriteWidth;
    idleBox.h = idleSpriteHeight;

    /* datos sprites para disparar */
    shotStartX = 1;
    shotStartY = 1;
    shotSpriteWidth = 62;
    shotSpriteHeight = 103;
    numberOfShotSprites = 15;

    shotBox.x = initialSreenPos.x;
    shotBox.y = initialSreenPos.y;
    shotBox.w = shotSpriteWidth;
    shotBox.h = shotSpriteHeight;
}

