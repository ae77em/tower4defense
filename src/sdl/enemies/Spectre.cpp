#include "Spectre.h"
#include "../Utils.h"

Spectre::Spectre(int x, int y, SDL_Renderer *renderer, Texture *texture)
    : Enemy(model::Enemy(100, 6, "spectre"), x, y, renderer, texture) {
    currentDirection = 0;
    initializeSpritesData();
}

void Spectre::initializeSpritesData() {
    /* datos sprites para caminar */
    walkingStartX = 1;
    walkingStartY = 1;
    walkSpriteWidth = 221;
    walkSpriteHeight = 221;
    numberOfEnemyWalkSprites = 10;// NUMBER_OF_ENEMY_WALK_SPRITES;
    numberOfEnemyWalkDirections = 4; // NUMBER_OF_ENEMY_WALK_DIRECTIONS;

    walkBox.x = currentPoint.x;
    walkBox.y = currentPoint.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 0;
    deathSpriteWidth = 221;
    deathSpriteHeight = 221;
    numberOfEnemyDeathSprites = 10; // NUMBER_OF_ENEMY_DEATH_SPRITES;
    numberOfEnemyDeathDirections = 4; // NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    deathBox.x = currentPoint.x;
    deathBox.y = currentPoint.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 1;
}

