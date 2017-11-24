#include "BloodHawk.h"
#include "../Utils.h"

BloodHawk::BloodHawk(int x,
                     int y,
                     SDL_Renderer *renderer,
                     Texture *texture)
        : Enemy(x, y, renderer, texture) {
    currentDirection = 0;
    initializeSpritesData();
}

void BloodHawk::initializeSpritesData() {
    /* datos sprites para caminar */
    walkingStartX = 1277;
    walkingStartY = 1336;
    walkSpriteWidth = 99;
    walkSpriteHeight = 76;
    numberOfEnemyWalkSprites = 8;
    numberOfEnemyWalkDirections = 4;

    walkBox.x = currentPoint.x;
    walkBox.y = currentPoint.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 13;
    deathSpriteWidth = 118;
    deathSpriteHeight = 96;
    numberOfEnemyDeathSprites = 22;
    numberOfEnemyDeathDirections = 4;

    deathBox.x = currentPoint.x;
    deathBox.y = currentPoint.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 0;
}

