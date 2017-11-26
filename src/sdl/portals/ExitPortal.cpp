#include "ExitPortal.h"
#include "../Constants.h"
#include "../Utils.h"

ExitPortal::ExitPortal(int x,
                         int y,
                         SDL_Renderer *renderer,
                         Texture *texture) :
        currentPoint(Utils::mapToScreen(x, y)) {
    this->renderer = renderer;
    this->texture = texture;
    drawBox.x = x;
    drawBox.y = y;
    setSprites();
}

void ExitPortal::setSprites() {
    drawBox.h = redSpriteHeight;
    drawBox.w = redSpriteWidth;


    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 10; ++j){
            spriteClipsPortalRed[10*i+j].x = j * redSpriteWidth;
            spriteClipsPortalRed[10*i+j].y = i * redSpriteHeight;
            spriteClipsPortalRed[10*i+j].w = redSpriteWidth;
            spriteClipsPortalRed[10*i+j].h = redSpriteHeight;
        }
    }
}

void ExitPortal::render(SDL_Rect &camera) {
    int frameToDraw = (SDL_GetTicks() / 100) % numberOfPortalSprites;
    DecimalPoint screenPoint = Utils::cartesianToIso(drawBox.x, drawBox.y);

    int offset = drawBox.h - ISO_TILE_HEIGHT;

    double isox = screenPoint.x - camera.x;
    double isoy = screenPoint.y - camera.y - offset;

    texture->renderSprite(renderer,
                          isox,
                          isoy,
                          &spriteClipsPortalRed[frameToDraw]);
}

void ExitPortal::animate(SDL_Rect &camera) {
    render(camera);
}

ExitPortal::~ExitPortal() { }

Point ExitPortal::getPoint() {
    return currentPoint;
}
