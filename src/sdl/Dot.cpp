#include "Dot.h"
#include "Constants.h"
#include "../common/Point.h"
#include "Utils.h"
#include "Keybinding.h"

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
        if (e.key.keysym.sym == default_keybinding.up)
            mVelY -= DOT_VEL;
        else if (e.key.keysym.sym == default_keybinding.down)
            mVelY += DOT_VEL;
        else if (e.key.keysym.sym == default_keybinding.left)
            mVelX -= DOT_VEL;
        else if (e.key.keysym.sym == default_keybinding.right)
            mVelX += DOT_VEL;
        else
            desc = "";
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        //Adjust the velocity
        desc = "solte boton";
        if (e.key.keysym.sym == default_keybinding.up)
            mVelY += DOT_VEL;
        else if (e.key.keysym.sym == default_keybinding.down)
            mVelY -= DOT_VEL;
        else if (e.key.keysym.sym == default_keybinding.left)
            mVelX += DOT_VEL;
        else if (e.key.keysym.sym == default_keybinding.right)
            mVelX -= DOT_VEL;
        else
            desc = "";
    }
}

void Dot::move() {
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right
    int width = (TILES_ROWS + 2) * ISO_TILE_WIDTH;
    if ((mBox.x < -width) || (mBox.x + DOT_WIDTH > width)) {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down
    int height = (TILES_COLUMNS - 2) * ISO_TILE_HEIGHT;
    if ((mBox.y < -height) || (mBox.y + DOT_HEIGHT > height)) {
        //move back
        mBox.y -= mVelY;
    }
}

void Dot::setCamera(SDL_Rect &camera) {
    //Center the camera over the dot
    camera.x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    int max_y_pos = ((TILES_COLUMNS - 2) * ISO_TILE_HEIGHT) - camera.h;
    int max_x_pos = ((TILES_ROWS + 2) * ISO_TILE_WIDTH) - camera.w;

    //Keep the camera in bounds
    if (camera.x < -max_x_pos) {
        camera.x = -max_x_pos;
    }
    if (camera.y < -max_y_pos) {
        camera.y = -max_y_pos;
    }
    if (camera.x > max_x_pos) {
        camera.x = max_x_pos;
    }
    if (camera.y > max_y_pos) {
        camera.y = max_y_pos;
    }
}

void Dot::render(LTexture &gDotTexture, SDL_Rect &camera, SDL_Renderer
*gRenderer) {
    //Show the dot
    gDotTexture.render(gRenderer, mBox.x - camera.x, mBox.y - camera.y);
}
