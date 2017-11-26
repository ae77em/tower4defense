#include <sys/param.h>
#include "Dot.h"
#include "Constants.h"
#include "../common/Protocol.h"

Dot::Dot(const Keybinding &keys) : mBox{0, 0, DOT_WIDTH, DOT_HEIGHT},
                                   mVelX(0), mVelY(0), keys(keys) {}

void Dot::handleEvent(SDL_Event &e) {
    // Increase speed on keydown
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        if (e.key.keysym.sym == keys.up)
            mVelY -= DOT_VEL;
        else if (e.key.keysym.sym == keys.down)
            mVelY += DOT_VEL;
        else if (e.key.keysym.sym == keys.left)
            mVelX -= DOT_VEL;
        else if (e.key.keysym.sym == keys.right)
            mVelX += DOT_VEL;

    // Reduce speed on keyup
    } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
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

    // Keep dot in horizontal bounds
    //TODO: remove this limit
    int width = 100000;
    if ((mBox.x < -width) || (mBox.x + DOT_WIDTH > width))
        mBox.x -= mVelX;

    //Move the dot up or down
    mBox.y += mVelY;

    // Keep dot in vertical bounds
    //TODO: remove this limit
    int height = 100000;
    if ((mBox.y < -height) || (mBox.y + DOT_HEIGHT > height))
        mBox.y -= mVelY;
}

void Dot::setCamera(SDL_Rect &camera) {
    //Center the camera over the dot
    camera.x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    //FIXME: possible duplicate with limits in Dot::move
    if (camera.x < -100000) {
        camera.x = -100000;
    }
    if (camera.y < -CARTESIAN_TILE_HEIGHT) {
        camera.y = -CARTESIAN_TILE_HEIGHT;
    }
    if (camera.x > 100000){
        camera.x = 100000;
    }
    if (camera.y > 100000) {
        camera.y = 100000;
    }
}

void
Dot::render(Texture &gDotTexture, SDL_Rect &camera, SDL_Renderer *gRenderer) {
    gDotTexture.render(gRenderer, mBox.x - camera.x, mBox.y - camera.y);
}
