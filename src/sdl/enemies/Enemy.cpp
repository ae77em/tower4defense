#include "Enemy.h"
#include "../Utils.h"

Enemy::Enemy(int x, int y, SDL_Renderer *r, LTexture *t) : currentPoint(Utils::mapToScreen(x, y)) {
    initializeSpritesData();

    collisionCircle.r = Enemy::getCollisionCircleRadio();
    shiftColliders();

    texture = t;

    //Initialize the velocity
    velocity = 1;
    initialLifePoints = remainingLifePoints = 200;
    isAir = false;
    isAlive = true;

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
    numberOfEnemyWalkSprites = 12;// NUMBER_OF_ENEMY_WALK_SPRITES;
    numberOfEnemyWalkDirections = 4; // NUMBER_OF_ENEMY_WALK_DIRECTIONS;

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
    numberOfEnemyDeathSprites = 18;// NUMBER_OF_ENEMY_DEATH_SPRITES;
    numberOfEnemyDeathDirections = 4; // NUMBER_OF_ENEMY_DEATH_DIRECTIONS;

    currentDirection = 0;
}

Enemy::~Enemy() {}

bool Enemy::loadMedia() {
    bool success = true;
    if (!texture->loadFromFile("images/sprites/enemy-abominable.png", renderer, 0xFF, 0x00, 0x99)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }
    return success;
}

void Enemy::kill() {
    isAlive = false;
}

void Enemy::setPosition(int x, int y) {
    currentPoint = Utils::mapToScreen(x, y);
    walkBox.x = currentPoint.x;
    walkBox.y = currentPoint.y;
}

void Enemy::setSprites() {
    int spriteWidth = walkSpriteWidth + separationBetweenSprites;
    int spriteHeight = walkSpriteHeight + separationBetweenSprites;

    int spritesRow;

    for (int i = 0; i < numberOfEnemyWalkDirections; ++i) {
        for (int j = 0; j < numberOfEnemyWalkSprites; ++j) {
            spritesRow = SPRITE_DIRECTIONS[i];

            walkingSprites[i][j].x = walkingStartX + (j * spriteWidth);
            walkingSprites[i][j].y = walkingStartY + (spritesRow * spriteHeight);
            walkingSprites[i][j].w = walkSpriteWidth;
            walkingSprites[i][j].h = walkSpriteHeight;
        }
    }

    // seteo los sprites para morir...
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

void Enemy::renderDie(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % numberOfEnemyDeathSprites;

    firstFrameOfDeathRendered = (firstFrameOfDeathRendered || frameToDraw == 0);
    lastFrameOfDeathRendered = (lastFrameOfDeathRendered || frameToDraw == NUMBER_OF_ENEMY_DEATH_SPRITES - 1);

    //if (!(firstFrameOfDeathRendered and lastFrameOfDeathRendered)) {
        DecimalPoint screenPoint = Utils::cartesianToIso(deathBox.x, deathBox.y);

        int offset = walkBox.h - ISO_TILE_HEIGHT;

        double isox = screenPoint.x - camera.x;
        double isoy = screenPoint.y - camera.y - offset;

        texture->renderSprite(renderer, isox, isoy, &deathSprites[currentDirection][frameToDraw]);
    //}
}

void Enemy::animate(SDL_Rect &camera) {
    if (isAlive) {
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

void Enemy::move() {
    walkBox.x += velocityX;

    // stop if is the end is reached
    if ((walkBox.x < 0) or (walkBox.x > TILES_COLUMNS * CARTESIAN_TILE_WIDTH)) {
        //move back
        walkBox.x -= velocityX;
    }

    walkBox.y += velocityY;

    // stop if is the end is reached
    if ((walkBox.y < 0) || (walkBox.y > TILES_ROWS * CARTESIAN_TILE_HEIGHT)) {
        //move back
        walkBox.y -= velocityY;
    }
}

const SDL_Rect &Enemy::getWalkBox() const {
    return walkBox;
}

int Enemy::getLifePoints() const {
    return initialLifePoints;
}

int Enemy::getVelocity() const {
    return velocity;
}

int Enemy::getIsAir() const {
    return isAir;
}

bool Enemy::itIsAlive() const {
    return isAlive;
}

void Enemy::quitLifePoints(int points) {
    remainingLifePoints -= points;

    if (remainingLifePoints <= 0) {
        isAlive = false;
    }
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
    int w = 50; // porque sí
    int h = 4; // porque también (?)...
    double percent = double(remainingLifePoints) / double(initialLifePoints);
    SDL_Color FGColor = {0x00, 0xFF, 0x00, 0xFF}; // green
    SDL_Color BGColor = {0xFF, 0x00, 0x00, 0xFF}; // red

    percent = percent > 1.f ? 1.f : percent < 0.f ? 0.f : percent;
    SDL_Color old;
    SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.g, &old.a);
    SDL_Rect bgrect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, BGColor.r, BGColor.g, BGColor.b, BGColor.a);
    SDL_RenderFillRect(renderer, &bgrect);
    SDL_SetRenderDrawColor(renderer, FGColor.r, FGColor.g, FGColor.b, FGColor.a);
    int pw = (int) ((float) w * percent);
    int px = x + (w - pw);
    SDL_Rect fgrect = {px, y, pw, h};
    SDL_RenderFillRect(renderer, &fgrect);
    SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);
}

int Enemy::getBonus() {
    return initialLifePoints / 2;
}

int Enemy::getCollisionCircleRadio(){
    return CARTESIAN_TILE_WIDTH / 2;
}

void Enemy::setTexture(LTexture *t) {
    texture = t;
}

void Enemy::setRenderer(SDL_Renderer *r) {
    renderer = r;
}

Point Enemy::getPoint() {
    return currentPoint;
}

