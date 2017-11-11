#include <iostream>
#include "BloodHawk.h"
#include "../Utils.h"

BloodHawk::BloodHawk(int x, int y, SDL_Renderer *renderer, LTexture *texture)
        : Enemy(model::Enemy(100, 4, "bloodhawk"), x, y, renderer, texture) {
    currentDirection = 0;
    initializeSpritesData();
}

BloodHawk::~BloodHawk() {}

void BloodHawk::initializeSpritesData() {
    /* datos sprites para caminar */
    walkingStartX = 1277;
    walkingStartY = 1336;
    walkSpriteWidth = 99;
    walkSpriteHeight = 76;
    numberOfEnemyWalkSprites = 8;// NUMBER_OF_ENEMY_WALK_SPRITES;
    numberOfEnemyWalkDirections = 4; // NUMBER_OF_ENEMY_WALK_DIRECTIONS;

    walkBox.x = currentPoint.x;
    walkBox.y = currentPoint.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 13;
    deathSpriteWidth = 118;
    deathSpriteHeight = 96;
    numberOfEnemyDeathSprites = 22;// NUMBER_OF_ENEMY_DEATH_SPRITES;
    numberOfEnemyDeathDirections = 4; // NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    deathBox.x = currentPoint.x;
    deathBox.y = currentPoint.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 0;
}

