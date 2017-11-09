#include "Goatman.h"
#include "../Utils.h"

Goatman::Goatman(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 100;
    isAir = false;
    velocity = 2;
}

void Goatman::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para caminar */
    walkingStartX = 1376;
    walkingStartY = 1681;
    walkSpriteWidth = 82;
    walkSpriteHeight = 79;
    numberOfEnemyWalkSprites = 8;// NUMBER_OF_ENEMY_WALK_SPRITES;
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
}

