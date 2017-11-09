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
}


bool BloodHawk::loadMedia() {
    bool success = true;
    if (!texture->loadFromFile("images/sprites/enemy-blood-hawk.png", renderer, 0xAA, 0xAA, 0xAA)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }
    return success;
}

void BloodHawk::setSprites() {
    int spriteWidth = this->walkSpriteWidth;
    int spriteHeight = this->walkSpriteHeight;

    int spritesRow;

    for (int i = 0; i < this->numberOfEnemyWalkDirections; ++i) {
        for (int j = 0; j < this->numberOfEnemyWalkSprites; ++j) {
            spritesRow = SPRITE_DIRECTIONS[i];

            walkingSprites[i][j].x = walkingStartX + (j * spriteWidth);
            walkingSprites[i][j].y = walkingStartY + (spritesRow * spriteHeight);
            walkingSprites[i][j].w = walkSpriteWidth;
            walkingSprites[i][j].h = walkSpriteHeight;
        }
    }

    // seteo los sprites para morir...
    spriteWidth = deathSpriteWidth;
    spriteHeight = deathSpriteHeight;

    for (int i = 0; i < numberOfEnemyDeathDirections; ++i) {
        for (int j = 0; j < numberOfEnemyDeathSprites; ++j) {
            spritesRow = SPRITE_DIRECTIONS[i];

            deathSprites[i][j].x = deathStartX + (j * spriteWidth);
            deathSprites[i][j].y = deathStartY + (spritesRow * spriteHeight);
            deathSprites[i][j].w = deathSpriteWidth;
            deathSprites[i][j].h = deathSpriteHeight;
        }
    }
}

void BloodHawk::renderWalk(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % numberOfEnemyWalkSprites;
    DecimalPoint screenPoint = Utils::cartesianToIso(walkBox.x, walkBox.y);

    // set the death box pos to assure that the death start in the same
    // tile in that the walk ended...
    deathBox.x = walkBox.x;
    deathBox.y = walkBox.y;

    int offset = walkBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    if (currentDirection > 7){
        currentDirection = 0;
    }

    texture->renderSprite(renderer, isox, isoy, &walkingSprites[currentDirection][frameToDraw]);

    renderLifeBar(isox, isoy);
}
