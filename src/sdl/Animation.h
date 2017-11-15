/* Shows the appropiate frame of an animation
 *
 * hcount and vcount are, respectively, the count of horizontal
 * and vertical frames on the spritesheet. The x,y position given
 * is relative to the 0,0 position of the screen, not the map,
 * and is not isometric.
 */

#include <SDL2/SDL.h>
#include "Texture.h"

#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
    SDL_Renderer *renderer;
    SDL_Rect clip;
    Texture spritesheet;
    unsigned hcount, vcount;

    public:
    Animation(SDL_Renderer *renderer, const std::string &filename,
            unsigned hcount, unsigned vcount);

    void renderFrame(int frame, int x, int y);

    int getHeight();
    int getWidth();
};

#endif
