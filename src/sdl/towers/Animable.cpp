#include "Tower.h"
#include "../Utils.h"

Tower::Tower(int x, int y, SDL_Renderer *r, LTexture &t) : texture(t), currentPoint(Utils::mapToScreen(x, y)) {
    idleBox.x = currentPoint.x;
    idleBox.y = currentPoint.y;
    idleBox.w = IDLE_SPRITE_WIDTH;
    idleBox.h = IDLE_SPRITE_HEIGHT;

    shotBox = idleBox;
    idleBox.w = SHOT_SPRITE_WIDTH;
    idleBox.h = SHOT_SPRITE_HEIGHT;

    shiftColliders();

    isShooting = false;

    renderer = r;

    setSprites();
}

Tower::~Tower(){}

bool Tower::loadMedia() {
    bool success = true;
    if (!texture.loadFromFile("images/sprites/tower-earth.png", renderer, 0xFF, 0x00, 0x99)) {
        printf("Failed to load dot texture!\n");
        success = false;
    }
    return success;
}

void Tower::setPosition(int x, int y) {
    idleBox.x = x;
    idleBox.y = y;

    shiftColliders();
}

void Tower::setSprites() {
    for (int i = 0; i < numberOfIdleSprites; ++i) {
        idleSprites[i].x = idleStartX + (i * idleSpriteWidth);
        idleSprites[i].y = idleStartY;
        idleSprites[i].w = IDLE_SPRITE_WIDTH;
        idleSprites[i].h = IDLE_SPRITE_HEIGHT;
    }

    // seteo los sprites para disparo...
    for (int i = 0; i < numberOfShotSprites; ++i) {
        shotSprites[i].x = shotStartX + (i * shotSpriteWidth);
        shotSprites[i].y = shotStartY;
        shotSprites[i].w = shotSpriteWidth;
        shotSprites[i].h = shotSpriteHeight;
    }
}

void Tower::animate(SDL_Rect &camera) {
    if (isShooting) {
        renderShot(camera);
    } else {
        renderIdle(camera);
    }
}

void Tower::setIsShooting(bool isShooting) {
    Tower::isShooting = isShooting;
}

void Tower::renderIdle(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % NUMBER_OF_IDLE_SPRITES;
    DecimalPoint screenPoint = Utils::cartesianToIso(idleBox.x, idleBox.y);

    // set the shot box pos to assure that the shot start in the same
    // tile in that the idle ended...
    shotBox.x = idleBox.x;
    shotBox.y = idleBox.y;

    int xoffset = (idleBox.w - ISO_TILE_WIDTH) / 2;
    int yoffset = idleBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x - xoffset;
    double isoy = screenPoint.y - camera.y - yoffset;

    texture.renderSprite(renderer, isox, isoy, &idleSprites[frameToDraw]);
}

void Tower::renderShot(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % NUMBER_OF_SHOT_SPRITES;

    DecimalPoint screenPoint = Utils::cartesianToIso(shotBox.x, shotBox.y);

    int offset = idleBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    texture.renderSprite(renderer, isox, isoy, &shotSprites[frameToDraw]);
}

void Tower::shiftColliders() {
    // tanto el box de idle como el de disparo tienen las mismas dimensiones,
    // así que es indistinto qué medida tomamos
    collisionCircle.x = idleBox.x;
    collisionCircle.y = idleBox.y;
}

Circle &Tower::getCollisionCircle() {
    return collisionCircle;
}

Point Tower::getPoint() {
    return currentPoint;
}

const SDL_Rect & Tower::getIdleBox() const {
    return idleBox;
}