#include "Enemy.h"
#include "../Utils.h"

Enemy::Enemy(int x, int y, SDL_Renderer *r, LTexture &t) : texture(t) {
    Point initialSreenPos = Utils::mapToScreen(x, y);
    walkBox.x = initialSreenPos.x;
    walkBox.y = initialSreenPos.y;
    walkBox.w = WALK_SPRITE_WIDTH;
    walkBox.h = WALK_SPRITE_HEIGHT;

    deathBox = walkBox;
    walkBox.w = DEATH_SPRITE_WIDTH;
    walkBox.h = DEATH_SPRITE_HEIGHT;

    //Initialize the velocity
    velocityX = 5;
    velocityY = 0;

    renderer = r;

    setSprites();
}

Enemy::~Enemy() {}

bool Enemy::loadMedia() {
    bool success = true;
    if (!texture.loadFromFile("images/sprites/enemy-abominable.png", renderer, 0xFF, 0x00, 0x99)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }
    return success;
}

void Enemy::kill(){
    isAlive = false;
}

void Enemy::setPosition(int x, int y) {
    Point sreenPos = Utils::mapToScreen(x, y);
    walkBox.x = sreenPos.x;
    walkBox.y = sreenPos.y;
}

void Enemy::setSprites() {
    int spriteWidth = WALK_SPRITE_WIDTH + 1;
    int spriteHeight = WALK_SPRITE_HEIGHT + 1;

    // seteo los sprites para caminar...
    int spritesRow;
    int walkingStartX = 1765;
    int walkingStartY = 3537;

    for (int i = 0; i < NUMBER_OF_WALK_DIRECTIONS; ++i) {
        for (int j = 0; j < NUMBER_OF_WALK_SPRITES; ++j) {
            spritesRow = SPRITE_DIRECTIONS[i];

            walkingSprites[i][j].x = walkingStartX + (j * spriteWidth);
            walkingSprites[i][j].y = walkingStartY + (spritesRow * spriteHeight);
            walkingSprites[i][j].w = WALK_SPRITE_WIDTH;
            walkingSprites[i][j].h = WALK_SPRITE_HEIGHT;
        }
    }

    // seteo los sprites para morir...
    spriteWidth = DEATH_SPRITE_WIDTH + 1;
    spriteHeight = DEATH_SPRITE_HEIGHT + 1;

    int deathStartX = 1;
    int deathStartY = 2273;

    for (int i = 0; i < NUMBER_OF_DEATH_DIRECTIONS; ++i) {
        for (int j = 0; j < NUMBER_OF_DEATH_SPRITES; ++j) {
            spritesRow = SPRITE_DIRECTIONS[i];

            deathSprites[i][j].x = deathStartX + (j * spriteWidth);
            deathSprites[i][j].y = deathStartY + (spritesRow * spriteHeight);
            deathSprites[i][j].w = DEATH_SPRITE_WIDTH;
            deathSprites[i][j].h = DEATH_SPRITE_HEIGHT;
        }
    }
}

void Enemy::renderWalk(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % NUMBER_OF_WALK_SPRITES;
    DecimalPoint screenPoint = Utils::cartesianToIso(walkBox.x, walkBox.y);

    // set the death box pos to assure that the death start in the same
    // tile in that the walk ended...
    deathBox.x = walkBox.x;
    deathBox.y = walkBox.y;

    int offset = walkBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    texture.renderSprite(renderer, isox, isoy, &walkingSprites[3][frameToDraw]);
}

void Enemy::renderDie(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % NUMBER_OF_DEATH_SPRITES;

    DecimalPoint screenPoint = Utils::cartesianToIso(deathBox.x, deathBox.y);

    int offset = walkBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    texture.renderSprite(renderer, isox, isoy, &deathSprites[3][frameToDraw]);
}

void Enemy::animate(SDL_Rect &camera) {
    if (isAlive){
        renderWalk(camera);
    } else {
        renderDie(camera);
    }
}

void Enemy::move() {
    Point minXScreenPoint = Utils::mapToScreen(0, TILES_ROWS);
    Point maxXScreenPoint = Utils::mapToScreen(TILES_COLUMNS, 0);
    Point minYScreenPoint = Utils::mapToScreen(0, 0);
    Point maxYScreenPoint = Utils::mapToScreen(TILES_COLUMNS, TILES_ROWS);

    walkBox.x += velocityX;

    // stop if is the end is reached
    if ((walkBox.x < minXScreenPoint.x) or (walkBox.x > maxXScreenPoint.x)) {
        //move back
        walkBox.x -= velocityX;
    }

    walkBox.y += velocityY;

    // stop if is the end is reached
    if ((walkBox.y < minYScreenPoint.y) || (walkBox.y > maxYScreenPoint.y)) {
        //move back
        walkBox.y -= velocityY;
    }
}

const SDL_Rect &Enemy::getWalkBox() const {
    return walkBox;
}
