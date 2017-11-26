#include "Spectre.h"
#include "SdlUtils.h"

Spectre::Spectre(int x, int y, SDL_Renderer *renderer, Texture *texture)
    : Enemy(x, y, renderer, texture) {
    currentDirection = 0;
    initializeSpritesData();
    setSprites();
}

void Spectre::initializeSpritesData() {
    /* datos sprites para caminar */
    walkingStartX = 1;
    walkingStartY = 1;
    walkSpriteWidth = 221;
    walkSpriteHeight = 221;
    numberOfEnemyWalkSprites = 10;
    numberOfEnemyWalkDirections = 4;

    walkBox.x = currentPoint.x;
    walkBox.y = currentPoint.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 0;
    deathSpriteWidth = 221;
    deathSpriteHeight = 221;
    numberOfEnemyDeathSprites = 10;
    numberOfEnemyDeathDirections = 4;

    deathBox.x = currentPoint.x;
    deathBox.y = currentPoint.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 1;
}

