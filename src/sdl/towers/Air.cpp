#include "Air.h"
#include "../Utils.h"

Air::Air(int x, int y, SDL_Renderer *renderer, LTexture &t) : Tower(x, y, renderer, t) {
    initializeSpritesData(x,y);
}

Air::~Air(){}

void Air::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para caminar */
    idleStartX = 1376;
    idleStartY = 1681;
    idleSpriteWidth = 82;
    idleSpriteHeight = 79;
    numberOfIdleSprites = 8;// NUMBER_OF_ENEMY_WALK_SPRITES;

    idleBox.x = initialSreenPos.x;
    idleBox.y = initialSreenPos.y;
    idleBox.w = idleSpriteWidth;
    idleBox.h = idleSpriteHeight;

    /* datos sprites para morir */
    shotStartX = 0;
    shotStartY = 2339;
    shotSpriteWidth = 160;
    shotSpriteHeight = 148;
    numberOfShotSprites = 19;// NUMBER_OF_ENEMY_DEATH_SPRITES;

    shotBox.x = initialSreenPos.x;
    shotBox.y = initialSreenPos.y;
    shotBox.w = shotSpriteWidth;
    shotBox.h = shotSpriteHeight;
    /*currentDirection = 0;
    separationBetweenSprites = 1;*/
}
