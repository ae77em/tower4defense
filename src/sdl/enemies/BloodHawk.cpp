#include "BloodHawk.h"
#include "../Utils.h"

BloodHawk::BloodHawk(int x, int y, SDL_Renderer *renderer, LTexture *texture) : Enemy(x, y, renderer, texture) {
    initialLifePoints = 100;
    velocity = 4;
    isAir = true;

    initializeSpritesData(x, y);
}

void BloodHawk::initializeSpritesData(int x, int y) {
    Point initialSreenPos = Utils::mapToScreen(x, y);

    /* datos sprites para caminar */
    walkingStartX = 1277;
    walkingStartY = 1336;
    walkSpriteWidth = 91;
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
}


bool BloodHawk::loadMedia() {
    bool success = true;
    if (!texture->loadFromFile("images/sprites/enemy-blood-hawk.png", renderer, 0xAA, 0xAA, 0xAA)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }
    return success;
}
