#include <sys/param.h>
#include "Dot.h"
#include "Constants.h"

Dot::Dot(const Keybinding &keys) : mBox{0, 0, DOT_WIDTH, DOT_HEIGHT},
                                   mVelX(0), mVelY(0), keys(keys) {}

void Dot::handleEvent(SDL_Event &e) {
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        //Adjust the velocity
        if (e.key.keysym.sym == keys.up)
            mVelY -= DOT_VEL;
        else if (e.key.keysym.sym == keys.down)
            mVelY += DOT_VEL;
        else if (e.key.keysym.sym == keys.left)
            mVelX -= DOT_VEL;
        else if (e.key.keysym.sym == keys.right)
            mVelX += DOT_VEL;
    }
        //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        //Adjust the velocity
        if (e.key.keysym.sym == keys.up)
            mVelY += DOT_VEL;
        else if (e.key.keysym.sym == keys.down)
            mVelY -= DOT_VEL;
        else if (e.key.keysym.sym == keys.left)
            mVelX += DOT_VEL;
        else if (e.key.keysym.sym == keys.right)
            mVelX -= DOT_VEL;
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

    //int max_x_pos = ((TILES_ROWS + 2) * ISO_TILE_WIDTH) - camera.w;

    //Keep the camera in bounds
    if (camera.x <
        -(((CARTESIAN_TILE_WIDTH * MAX(TILES_COLUMNS, TILES_ROWS)) / 2) +
          camera.w)) {
        camera.x = -(
                (CARTESIAN_TILE_WIDTH * MAX(TILES_COLUMNS, TILES_ROWS)) / 2 +
                camera.w);
    }
    if (camera.y < -CARTESIAN_TILE_HEIGHT) {
        camera.y = -CARTESIAN_TILE_HEIGHT;
    }
    if (camera.x >
        (CARTESIAN_TILE_WIDTH * MAX(TILES_COLUMNS, TILES_ROWS)) / 2) {
        camera.x = (CARTESIAN_TILE_WIDTH * MAX(TILES_COLUMNS, TILES_ROWS)) / 2;
    }
    if (camera.y > CARTESIAN_TILE_HEIGHT * MAX(TILES_COLUMNS, TILES_ROWS)) {
        camera.y = CARTESIAN_TILE_HEIGHT * MAX(TILES_COLUMNS, TILES_ROWS);
    }
}

void
Dot::render(LTexture &gDotTexture, SDL_Rect &camera, SDL_Renderer *gRenderer) {
    //Show the dot
    gDotTexture.render(gRenderer, mBox.x - camera.x, mBox.y - camera.y);
}
