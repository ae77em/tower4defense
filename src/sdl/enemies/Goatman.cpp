#include "Goatman.h"
#include "../Utils.h"

Goatman::Goatman(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 100;
    isAir = false;
    velocity = 2;

    currentDirection = 0;

    initializeSpritesData();
}

Goatman::~Goatman() {}

void Goatman::initializeSpritesData() {
    /* datos sprites para caminar */
    walkingStartX = 1376;
    walkingStartY = 1681;
    walkSpriteWidth = 82;
    walkSpriteHeight = 79;
    numberOfEnemyWalkSprites = 8;// NUMBER_OF_ENEMY_WALK_SPRITES;
    numberOfEnemyWalkDirections = 4; // NUMBER_OF_ENEMY_WALK_DIRECTIONS;

    walkBox.x = currentPoint.x;
    walkBox.y = currentPoint.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 2339;
    deathSpriteWidth = 160;
    deathSpriteHeight = 148;
    numberOfEnemyDeathSprites = 19;// NUMBER_OF_ENEMY_DEATH_SPRITES;
    numberOfEnemyDeathDirections = 4; // NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    deathBox.x = currentPoint.x;
    deathBox.y = currentPoint.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 1;
}

