#include <iostream>
#include "IsometricTile.h"
#include "Constants.h"
#include "Utils.h"

IsometricTile::IsometricTile(int x, int y) {
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = ISO_TILE_WIDTH;
    mBox.h = ISO_TILE_HEIGHT;

    //Get the tile type
    mType = 0;
}

void IsometricTile::render(SDL_Rect &camera,
                           SDL_Rect *gTileClips,
                           SDL_Renderer *gRenderer,
                           LTexture *gTileTextures) {
    Point screenPoint = Utils::mapToScreen(mBox.x, mBox.y);

    int offset = 0;
    if (mType == 1){
        mBox.h = 194;
        offset = 194 - 80;
        std::cout << "altura box: " << mBox.h << std::endl;
    }

    int isox = screenPoint.x - camera.x;
    int isoy = screenPoint.y - camera.y - offset;

    gTileTextures[mType].render(gRenderer,
                                isox,
                                isoy,
                                &gTileClips[mType]);
}


void IsometricTile::render_sprite(SDL_Rect &camera,
                           SDL_Rect *gTileClips,
                           SDL_Renderer *gRenderer,
                           LTexture *gTileTextures) {

    Point screenPoint = Utils::mapToScreen(mBox.x, mBox.y);

        int isox = screenPoint.x - camera.x;
        int isoy = screenPoint.y - camera.y;

    gTileTextures->render_sprite(gRenderer, isox, isoy, gTileClips);

}

void IsometricTile::handleEvent(SDL_Event &e, std::string &desc) {
    //If mouse event happened
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        mType = (mType == BUTTON_SPRITE_MOUSE_DOWN)
                            ? BUTTON_SPRITE_DEFAULT
                            : BUTTON_SPRITE_MOUSE_DOWN;
        desc.append("cambio tile...");
    }
}

int IsometricTile::getType() {
    return mType;
}

SDL_Rect IsometricTile::getBox() {
    return mBox;
}
