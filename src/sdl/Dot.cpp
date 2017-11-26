#include <sys/param.h>
#include "Dot.h"
#include "Constants.h"
#include "Protocol.h"

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
    mBox.x += mVelX;
    mBox.y += mVelY;
}

// Center the camera over the dot
void Dot::setCamera(SDL_Rect &camera) {
    camera.x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;
}

void
Dot::render(Texture &gDotTexture, SDL_Rect &camera, SDL_Renderer *gRenderer) {
    gDotTexture.render(gRenderer, mBox.x - camera.x, mBox.y - camera.y);
}
