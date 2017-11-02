#include "Enemy.h"
#include "../Utils.h"

Enemy::Enemy(int x, int y, SDL_Renderer *r, LTexture &t) : texture(t) {
    Point initialSreenPos = Utils::mapToScreen(x, y);
    drawBox.x = initialSreenPos.x;
    drawBox.y = initialSreenPos.y;
    drawBox.w = ENEMY_WIDTH;
    drawBox.h = ENEMY_HEIGHT;

    //Initialize the velocity
    velocityX = 1;
    velocityY = 0;

    renderer = r;

    setSprites();
}

Enemy::~Enemy() {}

bool Enemy::loadMedia() {
    bool success = true;
    if (!texture.loadFromFile("images/sprites/enemy-abominable-walk.png", renderer)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }
    return success;
}

void Enemy::setPosition(int x, int y) {
    Point sreenPos = Utils::mapToScreen(x, y);
    drawBox.x = sreenPos.x;
    drawBox.y = sreenPos.y;
}

void Enemy::setSprites() {
    int spriteWidth = ENEMY_WIDTH + 1;
    int walkingStartFrame = (NUMBER_OF_ABOMINABLE_SPRITES * 7 * spriteWidth); // TODO: esto es para caminar.
    for (int i = 0; i < NUMBER_OF_ABOMINABLE_SPRITES; ++i) {
        walkingSprites[i].x = walkingStartFrame + i * spriteWidth;
        walkingSprites[i].y = 0;
        walkingSprites[i].w = ENEMY_WIDTH;
        walkingSprites[i].h = ENEMY_HEIGHT;
    }
}

void Enemy::render(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % NUMBER_OF_ABOMINABLE_SPRITES;
    DecimalPoint screenPoint = Utils::cartesianToIso(drawBox.x, drawBox.y);

    int offset = drawBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    texture.renderSprite(renderer, isox, isoy, &walkingSprites[frameToDraw]);
}

void Enemy::move() {
    Point minXScreenPoint = Utils::mapToScreen(0, TILES_ROWS);
    Point maxXScreenPoint = Utils::mapToScreen(TILES_COLUMNS, 0);
    Point minYScreenPoint = Utils::mapToScreen(0, 0);
    Point maxYScreenPoint = Utils::mapToScreen(TILES_COLUMNS, TILES_ROWS);

    drawBox.x += velocityX;

    // stop if is the end is reached
    if ((drawBox.x < minXScreenPoint.x) or (drawBox.x > maxXScreenPoint.x)) {
        //move back
        drawBox.x -= velocityX;
    }

    drawBox.y += velocityY;

    // stop if is the end is reached
    if ((drawBox.y < minYScreenPoint.y) || (drawBox.y > maxYScreenPoint.y)) {
        //move back
        drawBox.y -= velocityY;
    }
}
