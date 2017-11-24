#include "Enemy.h"
#include "../Utils.h"

Enemy::Enemy(int x, int y, SDL_Renderer *r, Texture *t)
        : currentPoint(Utils::mapToScreen(x, y)) {
    initializeSpritesData();

    collisionCircle.r = Enemy::getCollisionCircleRadio();
    shiftColliders();
    texture = t;
    renderer = r;
    currentDirection = 0;

    setSprites();
}

void Enemy::initializeSpritesData() {
    /* datos sprites para caminar */
    walkSpriteWidth = 105;
    walkSpriteHeight = 119;

    walkBox.x = currentPoint.x;
    walkBox.y = currentPoint.y;
    walkBox.w = walkSpriteWidth;
    walkBox.h = walkSpriteHeight;

    walkingStartX = 1765;
    walkingStartY = 3537;
    walkSpriteWidth = 105;
    walkSpriteHeight = 119;
    numberOfEnemyWalkSprites = 12;
    numberOfEnemyWalkDirections = 4;

    /* datos sprites para morir */
    deathSpriteWidth = 200;
    deathSpriteHeight = 155;

    deathBox = walkBox;

    walkBox.w = deathSpriteWidth;
    walkBox.h = deathSpriteHeight;

    deathStartX = 1;
    deathStartY = 2273;
    deathSpriteWidth = 200;
    deathSpriteHeight = 155;
    numberOfEnemyDeathSprites = 18;
    numberOfEnemyDeathDirections = 4;

    currentDirection = 0;
}

Enemy::~Enemy() {}

bool Enemy::loadMedia() {
    bool success = true;
    if (!texture->loadFromFile("images/sprites/enemy-abominable.png",
                               renderer, 0xFF, 0x00, 0x99)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }
    return success;
}

void Enemy::kill() {
    isAlive = false;
}

void Enemy::setSprites() {
    int spriteWidth = walkSpriteWidth + separationBetweenSprites;
    int spriteHeight = walkSpriteHeight + separationBetweenSprites;

    int spritesRow;

    for (int i = 0; i < numberOfEnemyWalkDirections; ++i) {
        for (int j = 0; j < numberOfEnemyWalkSprites; ++j) {
            spritesRow = SPRITE_DIRECTIONS[i];

            walkingSprites[i][j].x = walkingStartX + (j * spriteWidth);
            walkingSprites[i][j].y = walkingStartY
                                     + (spritesRow * spriteHeight);
            walkingSprites[i][j].w = walkSpriteWidth;
            walkingSprites[i][j].h = walkSpriteHeight;
        }
    }

    // seteo los sprites para morir
    spriteWidth = deathSpriteWidth + separationBetweenSprites;
    spriteHeight = deathSpriteHeight + separationBetweenSprites;

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

void Enemy::renderWalk(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % numberOfEnemyWalkSprites;
    DecimalPoint screenPoint = Utils::cartesianToIso(walkBox.x, walkBox.y);

    // Mostrar animacion de movimiento y muerte en el mismo lugar
    deathBox.x = walkBox.x;
    deathBox.y = walkBox.y;

    int offset = walkBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    if (currentDirection > 3){
        currentDirection = 0;
    }

    texture->renderSprite(renderer, isox, isoy,
                          &walkingSprites[currentDirection][frameToDraw]);

    // Centrar la barra de vida
    int isoxLifebar = isox + ((walkBox.w - LIFE_BAR_WIDTH) / 2);

    renderLifeBar(isoxLifebar, isoy);
}

void Enemy::renderDie(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % numberOfEnemyDeathSprites;

    firstFrameOfDeathRendered = (firstFrameOfDeathRendered || frameToDraw == 0);
    lastFrameOfDeathRendered = (lastFrameOfDeathRendered
                 || frameToDraw == numberOfEnemyDeathSprites - 1);

    if (!(firstFrameOfDeathRendered and lastFrameOfDeathRendered)) {
        DecimalPoint screenPoint =
                Utils::cartesianToIso(deathBox.x, deathBox.y);

        int offset = walkBox.h - ISO_TILE_HEIGHT;

        double isox = screenPoint.x - camera.x;
        double isoy = screenPoint.y - camera.y - offset;

        texture->renderSprite(renderer, isox, isoy,
                              &deathSprites[currentDirection][frameToDraw]);
    }
}

void Enemy::animate(SDL_Rect &camera) {
    if (getIsAlive()) {
        renderWalk(camera);
    } else {
        renderDie(camera);
    }
}

void Enemy::moveTo(int x, int y) {
    walkBox.x = x;
    walkBox.y = y;
    shiftColliders();
}

void Enemy::setDirection(int d) {
    currentDirection = d;
}

void Enemy::move(int maxx, int maxy) {
    walkBox.x += velocityX;

    // stop if is the end is reached
    if ((walkBox.x < 0) || (walkBox.x > maxx * CARTESIAN_TILE_WIDTH)) {
        //move back
        walkBox.x -= velocityX;
    }

    walkBox.y += velocityY;

    // stop if is the end is reached
    if ((walkBox.y < 0) || (walkBox.y > maxy * CARTESIAN_TILE_HEIGHT)) {
        //move back
        walkBox.y -= velocityY;
    }
}

const SDL_Rect &Enemy::getWalkBox() const {
    return walkBox;
}

bool Enemy::getIsAlive() const {
    return isAlive;
}

void Enemy::shiftColliders() {
    //Align collider to center of dot
    collisionCircle.x = walkBox.x;
    collisionCircle.y = walkBox.y;
}

Circle &Enemy::getCollisionCircle() {
    return collisionCircle;
}

void Enemy::renderLifeBar(int x, int y) {
    int w = LIFE_BAR_WIDTH;
    int h = LIFE_BAR_HEIGHT;
    SDL_Color fcolor = {0x00, 0xFF, 0x00, 0xFF}; // green
    SDL_Color bcolor = {0xFF, 0x00, 0x00, 0xFF}; // red

    energyPercentaje = energyPercentaje > 1.f ? 1.f : energyPercentaje < 0.f ? 0.f : energyPercentaje;
    SDL_Color old;
    SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.g, &old.a);
    SDL_Rect bgrect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, bcolor.r, bcolor.g, bcolor.b, bcolor.a);
    SDL_RenderFillRect(renderer, &bgrect);
    SDL_SetRenderDrawColor(renderer, fcolor.r, fcolor.g, fcolor.b, fcolor.a);
    int pw = (int) ((float) w * energyPercentaje);
    int px = x + (w - pw);
    SDL_Rect fgrect = {px, y, pw, h};
    SDL_RenderFillRect(renderer, &fgrect);
    SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);
}

int Enemy::getCollisionCircleRadio(){
    return CARTESIAN_TILE_WIDTH / 2;
}

void Enemy::setTexture(Texture *t) {
    texture = t;
}

void Enemy::setRenderer(SDL_Renderer *r) {
    renderer = r;
}

Point Enemy::getPoint() {
    return currentPoint;
}

bool Enemy::itIsVisible() {
    return isVisible;
}

void Enemy::setIsVisible(bool isVisible) {
    Enemy::isVisible = isVisible;
}

void Enemy::setIsAlive(bool isAlive) {
    Enemy::isAlive = isAlive;
}


void Enemy::setEnergyPercentaje(double ep) {
    energyPercentaje = ep;
}

