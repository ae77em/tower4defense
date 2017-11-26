#include <Protocol.h>
#include "EnterPortal.h"
#include "Constants.h"
#include "CommonUtils.h"
#include "SdlUtils.h"

EnterPortal::EnterPortal(int x,
                         int y,
                         SDL_Renderer *renderer,
                         Texture *texture) :
        currentPoint(SdlUtils::mapToScreen(x, y)) {
    this->renderer = renderer;
    this->texture = texture;
    drawBox.x = x;
    drawBox.y = y;
    setSprites();
}

void EnterPortal::setSprites() {
    drawBox.h = blueSpriteHeight;
    drawBox.w = blueSpriteWidth;

    for (int i = 0; i < numberOfPortalSprites; ++i) {
        spriteClipsPortalBlue[i].x = i * blueSpriteWidth;
        spriteClipsPortalBlue[i].y = 0;
        spriteClipsPortalBlue[i].w = blueSpriteWidth;
        spriteClipsPortalBlue[i].h = blueSpriteHeight;
    }
}

void EnterPortal::render(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % numberOfPortalSprites;
    DecimalPoint screenPoint = SdlUtils::cartesianToIso(drawBox.x, drawBox.y);

    int offset = drawBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    texture->renderSprite(renderer,
                          isox,
                          isoy,
                          &spriteClipsPortalBlue[frameToDraw]);
}

void EnterPortal::animate(SDL_Rect &camera) {
    render(camera);
}

EnterPortal::~EnterPortal() { }

Point EnterPortal::getPoint() {
    return currentPoint;
}
