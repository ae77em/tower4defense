#include "Earth.h"
#include "../Utils.h"

Earth::Earth(int x, int y, SDL_Renderer *renderer, Texture &t)
        : Tower(x, y, renderer, t) {
    initializeSpritesData(x,y);
}

Earth::~Earth(){}

void Earth::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para iddle */
    idleStartX = 2;
    idleStartY = 568;
    idleSpriteWidth = 57;
    idleSpriteHeight = 99;
    numberOfIdleSprites = 17;

    idleBox.x = initialSreenPos.x;
    idleBox.y = initialSreenPos.y;
    idleBox.w = idleSpriteWidth;
    idleBox.h = idleSpriteHeight;

    /* datos sprites para disparar */
    shotStartX = 2;
    shotStartY = 18;
    shotSpriteWidth = 57;
    shotSpriteHeight = 99;
    numberOfShotSprites = 17;

    shotBox.x = initialSreenPos.x;
    shotBox.y = initialSreenPos.y;
    shotBox.w = shotSpriteWidth;
    shotBox.h = shotSpriteHeight;
}


