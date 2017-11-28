#include <Protocol.h>
#include "Air.h"
#include "SdlUtils.h"

Air::Air(int x, int y, SDL_Renderer *r, Texture &t)
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

Air::~Air(){}

void Air::setPosition(int x, int y) {
    idleBox.x = x;
    idleBox.y = y;
}

void Air::setSprites() {
    for (int i = 0; i < numberOfIdleSprites; ++i) {
        idleSprites[i].x = idleStartX + (i * idleSpriteWidth);
        idleSprites[i].y = idleStartY;
        idleSprites[i].w = IDLE_SPRITE_WIDTH;
        idleSprites[i].h = IDLE_SPRITE_HEIGHT;

        idleShineSprites[i].x = idleShineStartX + (i * 76);
        idleShineSprites[i].y = idleShineStartY;
        idleShineSprites[i].w = IDLE_SHINE_WIDTH;
        idleShineSprites[i].h = IDLE_SHINE_HEIGHT;
    }

    // seteo los sprites para disparo...
    for (int i = 0; i < numberOfShotSprites; ++i) {
        shotSprites[i].x = shotStartX + (i * shotSpriteWidth);
        shotSprites[i].y = shotStartY;
        shotSprites[i].w = shotSpriteWidth;
        shotSprites[i].h = shotSpriteHeight;

        shotShineSprites[i].x = shotShineStartX + (i * 106);
        shotShineSprites[i].y = shotShineStartY;
        shotShineSprites[i].w = 105;
        shotShineSprites[i].h = 115;
    }
}

void Air::animate(SDL_Rect &camera) {
    if (isShooting) {
        renderShot(camera);
    } else {
        renderIdle(camera);
    }
}

void Air::setIsShooting(bool isShooting) {
    Air::isShooting = isShooting;
}

void Air::renderIdle(SDL_Rect &camera) {
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

    texture.renderSprite(renderer,
                         isox,
                         isoy,
                         &idleSprites[frameToDraw]);
    texture.renderSprite(renderer,
                         isox - ((IDLE_SHINE_WIDTH - IDLE_SPRITE_WIDTH) / 2),
                         isoy - IDLE_SHINE_HEIGHT / 2,
                         &idleShineSprites[frameToDraw]);
}

void Air::renderShot(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % NUMBER_OF_SHOT_SPRITES;

    DecimalPoint screenPoint = SdlUtils::cartesianToIso(shotBox.x, shotBox.y);

    int offset = idleBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    texture.renderSprite(renderer, isox, isoy, &shotSprites[frameToDraw]);

    texture.renderSprite(renderer,
                         isox - ((105 - SHOT_SPRITE_WIDTH) / 2),
                         isoy - 115 / 2,
                         &shotShineSprites[frameToDraw]);
}

Point Air::getPoint() {
    return currentPoint;
}

const SDL_Rect & Air::getIdleBox() const {
    return idleBox;
}
