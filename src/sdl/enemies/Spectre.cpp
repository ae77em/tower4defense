#include "Spectre.h"
#include "../Utils.h"

Spectre::Spectre(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 100;
    isAir = true;
    velocity = 6;
    currentDirection = 0;

    initializeSpritesData(x, y);
}

void Spectre::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para caminar */
    walkingStartX = 1729;
    walkingStartY = 3364;
    walkSpriteWidth = 130;
    walkSpriteHeight = 112;
    numberOfEnemyWalkSprites = 10;// NUMBER_OF_ENEMY_WALK_SPRITES;
    numberOfEnemyWalkDirections = 4; // NUMBER_OF_ENEMY_WALK_DIRECTIONS;

    walkBox.x = initialSreenPos.x;
    walkBox.y = initialSreenPos.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 2339;
    deathSpriteWidth = 160;
    deathSpriteHeight = 148;
    numberOfEnemyDeathSprites = 19;// NUMBER_OF_ENEMY_DEATH_SPRITES;
    numberOfEnemyDeathDirections = 4; // NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    deathBox.x = initialSreenPos.x;
    deathBox.y = initialSreenPos.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 0;
}

