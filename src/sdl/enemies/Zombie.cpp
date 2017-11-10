#include "Zombie.h"
#include "../Utils.h"

Zombie::Zombie(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 20;
    isAir = false;
    velocity = 10;
    currentDirection = 0;

    initializeSpritesData(x, y);
}

void Zombie::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para caminar */
    walkingStartX = 1228;
    walkingStartY = 759;
    walkSpriteWidth = 101;
    walkSpriteHeight = 91;
    numberOfEnemyWalkSprites = 6;// NUMBER_OF_ENEMY_WALK_SPRITES;
    numberOfEnemyWalkDirections = 4; // NUMBER_OF_ENEMY_WALK_DIRECTIONS;

    walkBox.x = initialSreenPos.x;
    walkBox.y = initialSreenPos.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    /* datos sprites para morir */
    deathStartX = 0;
    deathStartY = 1505;
    deathSpriteWidth = 101;
    deathSpriteHeight = 91;
    numberOfEnemyDeathSprites = 15;// NUMBER_OF_ENEMY_DEATH_SPRITES;
    numberOfEnemyDeathDirections = 4; // NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    deathBox.x = initialSreenPos.x;
    deathBox.y = initialSreenPos.y;
    deathBox.w = deathSpriteWidth;
    deathBox.h = deathSpriteHeight;

    currentDirection = 0;
    separationBetweenSprites = 1;
}

