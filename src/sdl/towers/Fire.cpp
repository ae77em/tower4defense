#include <Protocol.h>
#include "Fire.h"
#include "SdlUtils.h"

Fire::Fire(int x, int y, SDL_Renderer *r, Texture &t)
        : Tower(x, y, renderer, texture),
          texture(t),
          currentPoint(SdlUtils::mapToScreen(x, y)) {

    idleSpriteWidth = IDLE_SPRITE_WIDTH + separationBetweenSprites;
    idleSpriteHeight = IDLE_SPRITE_HEIGHT + separationBetweenSprites;

    shotSpriteWidth = SHOT_SPRITE_WIDTH + separationBetweenSprites;
    shotSpriteHeight = SHOT_SPRITE_HEIGHT + separationBetweenSprites;

    numberOfIdleSprites = NUMBER_OF_IDLE_SPRITES;
    numberOfShotSprites = NUMBER_OF_SHOT_SPRITES;

    idleBox.x = currentPoint.x;
    idleBox.y = currentPoint.y;
    idleBox.w = IDLE_SPRITE_WIDTH;
    idleBox.h = IDLE_SPRITE_HEIGHT;

    shotBox = idleBox;
    idleBox.w = SHOT_SPRITE_WIDTH;
    idleBox.h = SHOT_SPRITE_HEIGHT;

    isShooting = false;

    renderer = r;

    setSprites();
}

Fire::~Fire(){}

void Fire::setPosition(int x, int y) {
    idleBox.x = x;
    idleBox.y = y;
}

void Fire::setSprites() {
    for (int i = 0; i < numberOfIdleSprites; ++i) {
        idleSprites[i].x = idleStartX + (i * idleSpriteWidth);
        idleSprites[i].y = idleStartY;
        idleSprites[i].w = IDLE_SPRITE_WIDTH;
        idleSprites[i].h = IDLE_SPRITE_HEIGHT;

        shineSprites[i].x = shineStartX + (i * 23);
        shineSprites[i].y = shineStartY;
        shineSprites[i].w = 22;
        shineSprites[i].h = 48;
    }
}

void Fire::animate(SDL_Rect &camera) {
    renderIdle(camera);
}

void Fire::setIsShooting(bool isShooting) {
    Fire::isShooting = isShooting;
}

void Fire::renderIdle(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % NUMBER_OF_IDLE_SPRITES;
    DecimalPoint screenPoint = SdlUtils::cartesianToIso(idleBox.x, idleBox.y);

    // set the shot box pos to assure that the shot start in the same
    // tile in that the idle ended...
    shotBox.x = idleBox.x;
    shotBox.y = idleBox.y;

    int xoffset = (idleBox.w - ISO_TILE_WIDTH) / 2;
    int yoffset = idleBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x - xoffset;
    double isoy = screenPoint.y - camera.y - yoffset;

    texture.renderSprite(renderer, isox, isoy, &idleSprites[frameToDraw]);
    texture.renderSprite(renderer,
                         isox + ((IDLE_SPRITE_WIDTH / 2) - 11),
                         isoy - 36,
                         &shineSprites[frameToDraw]);
}

Point Fire::getPoint() {
    return currentPoint;
}

const SDL_Rect & Fire::getIdleBox() const {
    return idleBox;
}

bool Fire::isFire(){
    return true;
}
