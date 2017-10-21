#include "IsometricTile.h"
#include "Constants.h"
#include "Utils.h"

IsometricTile::IsometricTile(int x, int y, int tileType) {
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = 0;
}

void IsometricTile::render(SDL_Rect &camera,
                           SDL_Rect *gTileClips,
                           SDL_Renderer *gRenderer,
                           LTexture *gTileTextures) {
    //If the tile is on screen
    if (Utils::checkCollision(camera, mBox)) {
        //Show the tile
        int isox = mBox.x - camera.x;
        int isoy = mBox.y - camera.y;

        gTileTextures[mType].render(gRenderer,
                                    isox,
                                    isoy,
                                    &gTileClips[mType]);
    }
}

void IsometricTile::handleEvent(SDL_Event &e, std::string &desc) {
    //If mouse event happened
    if (e.type == SDL_MOUSEMOTION
        || e.type == SDL_MOUSEBUTTONDOWN
        || e.type == SDL_MOUSEBUTTONUP) {
        //Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        //Check if mouse is in tile
        bool inside = true;

        //Mouse is left of the tile
        if (x < mBox.x) {
            inside = false;
        }
            //Mouse is right of the tile
        else if (x > mBox.x + TILE_WIDTH) {
            inside = false;
        }
            //Mouse above the tile
        else if (y < mBox.y) {
            inside = false;
        }
            //Mouse below the tile
        else if (y > mBox.y + TILE_HEIGHT) {
            inside = false;
        }

        //Mouse is outside tile
        if (!inside) {
//            mType = BUTTON_SPRITE_MOUSE_OUT;
        }
            //Mouse is inside tile
        else {
            //Set mouse over sprite
            switch (e.type) {
                /*case SDL_MOUSEMOTION:
                    mType = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;*/

                case SDL_MOUSEBUTTONDOWN:
                    mType = (mType == BUTTON_SPRITE_MOUSE_DOWN)
                            ? BUTTON_SPRITE_DEFAULT
                            : BUTTON_SPRITE_MOUSE_DOWN;
                    desc.append("cambio tile...");
                    break;

                    /*case SDL_MOUSEBUTTONUP:
                        mType = BUTTON_SPRITE_MOUSE_UP;
                        break;*/
            }
        }
    }
}

int IsometricTile::getType() {
    return mType;
}

SDL_Rect IsometricTile::getBox() {
    return mBox;
}
