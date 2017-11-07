#include "BloodHawk.h"

BloodHawk::BloodHawk(int x, int y, SDL_Renderer *renderer) : Enemy(x, y, renderer) {
    initialLifePoints = 100;
    velocity = 4;
    isAir = true;

    walkingStartX = 1277;
    walkingStartY = 1336;
    walkSpriteWidth = 91;
    walkSpriteHeight = 76;
    numberOfEnemyWalkSprites = 8;

    deathStartX = 0;
    deathStartY = 13;
    deathSpriteWidth = 118;
    deathSpriteHeight = 768;
    numberOfEnemyDeathSprites = 22;
}

bool BloodHawk::loadMedia() {
    bool success = true;
    if (!texture.loadFromFile("images/sprites/enemy-blood-hawk.png", renderer, 0xAA, 0xAA, 0xAA)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }
    return success;
}
