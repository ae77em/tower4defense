#include <iostream>
#include "BloodHawk.h"
#include "../Utils.h"

BloodHawk::BloodHawk(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 100;
    velocity = 4;
    isAir = true;

    currentDirection = 0;

    initializeSpritesData(x, y);
}

BloodHawk::~BloodHawk() {}

void BloodHawk::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para caminar */
    walkingStartX = 1277;
    walkingStartY = 1336;
    walkSpriteWidth = 99;
    walkSpriteHeight = 76;
    numberOfEnemyWalkSprites = 8;// NUMBER_OF_ENEMY_WALK_SPRITES;
    numberOfEnemyWalkDirections = 4; // NUMBER_OF_ENEMY_WALK_DIRECTIONS;

    walkBox.x = initialSreenPos.x;
    walkBox.y = initialSreenPos.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 13;
    deathSpriteWidth = 118;
    deathSpriteHeight = 96;
    numberOfEnemyDeathSprites = 22;// NUMBER_OF_ENEMY_DEATH_SPRITES;
    numberOfEnemyDeathDirections = 4; // NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    deathBox.x = initialSreenPos.x;
    deathBox.y = initialSreenPos.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 0;
}

