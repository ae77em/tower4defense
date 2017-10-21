#include "Dot.h"
#include "Constants.h"

Dot::Dot() {
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
    mBox.w = DOT_WIDTH;
    mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent(SDL_Event &e, std::string &desc) {
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        desc = "aprete boton";
        //Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY -= DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY += DOT_VEL;
                break;
            case SDLK_LEFT:
                mVelX -= DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX += DOT_VEL;
                break;
            default:
                desc = "";
        }
    }        //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        //Adjust the velocity
        desc = "solte boton";
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                mVelY += DOT_VEL;
                break;
            case SDLK_DOWN:
                mVelY -= DOT_VEL;
                break;
            case SDLK_LEFT:
                mVelX += DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX -= DOT_VEL;
                break;
            default:
                desc = "";
        }
    }
}

void Dot::move(IsometricTile *tiles[]) {
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right
    if ((mBox.x < 0)
        || (mBox.x + DOT_WIDTH > ((TILES_ROWS + 2) * TILE_WIDTH))) {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down
    if ((mBox.y < 0)
        || (mBox.y + DOT_HEIGHT > ((TILES_COLUMNS - 2) * TILE_HEIGHT))) {
        //move back
        mBox.y -= mVelY;
    }
}

void Dot::setCamera(SDL_Rect &camera) {
    //Center the camera over the dot
    camera.x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    if (camera.x < 0) {
        camera.x = 0;
    }
    if (camera.y < 0) {
        camera.y = 0;
    }
    if (camera.x > ((TILES_ROWS + 2) * TILE_WIDTH) - camera.w) {
        camera.x = ((TILES_ROWS + 2) * TILE_WIDTH) - camera.w;
    }
    if (camera.y > ((TILES_COLUMNS - 2) * TILE_HEIGHT) - camera.h) {
        camera.y = ((TILES_COLUMNS - 2) * TILE_HEIGHT) - camera.h;
    }
}

void Dot::render(LTexture &gDotTexture, SDL_Rect &camera, SDL_Renderer
*gRenderer) {
    //Show the dot
    gDotTexture.render(gRenderer, mBox.x - camera.x, mBox.y - camera.y);
}
