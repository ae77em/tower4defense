#include "GreenDaemon.h"
#include "../Utils.h"

GreenDaemon::GreenDaemon(int x,
                         int y,
                         SDL_Renderer *renderer,
                         Texture *texture)
        : Enemy(x, y, renderer, texture) {
    currentDirection = 0;
    initializeSpritesData();
}

void GreenDaemon::initializeSpritesData() {
    /* datos sprites para caminar */
    walkingStartX = 1729;
    walkingStartY = 3364;
    walkSpriteWidth = 130;
    walkSpriteHeight = 112;
    numberOfEnemyWalkSprites = 10;
    numberOfEnemyWalkDirections = 4;

    walkBox.x = currentPoint.x;
    walkBox.y = currentPoint.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 1357;
    deathSpriteWidth = 170;
    deathSpriteHeight = 119;
    numberOfEnemyDeathSprites = 16;
    numberOfEnemyDeathDirections = 4;

    deathBox.x = currentPoint.x;
    deathBox.y = currentPoint.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 0;
}
