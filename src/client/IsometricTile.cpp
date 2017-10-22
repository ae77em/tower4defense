#include <iostream>
#include "IsometricTile.h"
#include "Constants.h"
#include "Utils.h"

IsometricTile::IsometricTile(int x, int y) {
    //Get the offsets
    containerBoxAttributes.x = x;
    containerBoxAttributes.y = y;

    //Set the collision box
    containerBoxAttributes.w = ISO_TILE_WIDTH;
    containerBoxAttributes.h = ISO_TILE_HEIGHT;

    //Get the tile type
    type = 0;
}

void IsometricTile::render(SDL_Rect &camera,
                           SDL_Rect *gTileClips,
                           SDL_Renderer *gRenderer,
                           LTexture *gTileTextures) {

    Point screenPoint = Utils::mapToScreen(containerBoxAttributes.x, containerBoxAttributes.y);

    SDL_Rect aux = Utils::getBoxByTileType(type);

    int offset = aux.h - ISO_TILE_HEIGHT;

    int isox = screenPoint.x - camera.x;
    int isoy = screenPoint.y - camera.y - offset;

    gTileTextures[type].render(gRenderer,
                                isox,
                                isoy,
                                &gTileClips[type]);
}


void IsometricTile::render_sprite(SDL_Rect &camera,
                           SDL_Rect *gTileClips,
                           SDL_Renderer *gRenderer,
                           LTexture *gTileTextures) {

    Point screenPoint = Utils::mapToScreen(containerBoxAttributes.x, containerBoxAttributes.y);

        int isox = screenPoint.x - camera.x;
        int isoy = screenPoint.y - camera.y;

    gTileTextures->render_sprite(gRenderer, isox, isoy, gTileClips);

}

void IsometricTile::handleEvent(SDL_Event &e, std::string &desc) {
    //If mouse event happened
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        type = (type == BUTTON_SPRITE_MOUSE_DOWN)
                            ? BUTTON_SPRITE_DEFAULT
                            : BUTTON_SPRITE_MOUSE_DOWN;
        desc.append("cambio tile...");
    }
}

int IsometricTile::getType() {
    return type;
}

SDL_Rect IsometricTile::getBox() {
    return containerBoxAttributes;
}
