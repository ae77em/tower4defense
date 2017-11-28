#include <Protocol.h>
#include "Water.h"
#include "SdlUtils.h"

Water::Water(int x, int y, SDL_Renderer *r, Texture &t)
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

Water::~Water(){}

void Water::setPosition(int x, int y) {
    idleBox.x = x;
    idleBox.y = y;
}

void Water::setSprites() {
    for (int i = 0; i < numberOfIdleSprites; ++i) {
        idleSprites[i].x = idleStartX + (i * idleSpriteWidth);
        idleSprites[i].y = idleStartY;
        idleSprites[i].w = IDLE_SPRITE_WIDTH;
        idleSprites[i].h = IDLE_SPRITE_HEIGHT;

        idleShineSprites[i].x = idleShineStartX + (i * 18);
        idleShineSprites[i].y = idleShineStartY;
        idleShineSprites[i].w = 17;
        idleShineSprites[i].h = 17;

        idleShadowSprites[i].x = idleShadowStartX + (i * 40);
        idleShadowSprites[i].y = idleShadowStartY;
        idleShadowSprites[i].w = 39;
        idleShadowSprites[i].h = 40;
    }

    // seteo los sprites para disparo...
    for (int i = 0; i < numberOfShotSprites; ++i) {
        shotSprites[i].x = shotStartX + (i * shotSpriteWidth);
        shotSprites[i].y = shotStartY;
        shotSprites[i].w = shotSpriteWidth;
        shotSprites[i].h = shotSpriteHeight;
    }
}

void Water::animate(SDL_Rect &camera) {
    if (isShooting) {
        renderShot(camera);
    } else {
        renderIdle(camera);
    }
}

void Water::setIsShooting(bool isShooting) {
    Water::isShooting = isShooting;
}

void Water::renderIdle(SDL_Rect &camera) {
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

    int shadowX = isox + ((IDLE_SPRITE_WIDTH / 2) - 20);
    int shadowY = isoy - 30;
    texture.renderSprite(renderer, shadowX, shadowY, &idleShadowSprites[frameToDraw]);

    texture.renderSprite(renderer,
                         shadowX + 12,
                         shadowY + 12,
                         &idleShineSprites[frameToDraw]);
}

void Water::renderShot(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % NUMBER_OF_SHOT_SPRITES;

    DecimalPoint screenPoint = SdlUtils::cartesianToIso(shotBox.x, shotBox.y);

    int offset = idleBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    texture.renderSprite(renderer, isox, isoy, &shotSprites[frameToDraw]);
}

Point Water::getPoint() {
    return currentPoint;
}

const SDL_Rect & Water::getIdleBox() const {
    return idleBox;
}

bool Water::isWater() {
    return true;
}
