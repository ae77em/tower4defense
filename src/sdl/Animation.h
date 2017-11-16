#include <SDL2/SDL.h>
#include "Texture.h"

#ifndef ANIMATION_H
#define ANIMATION_H

/* Shows the appropiate frame of an animation */
class Animation {
    public:
    virtual ~Animation() {}
    virtual void renderFrame(int frame, int x, int y) = 0;
    virtual int getHeight() const = 0;
    virtual int getWidth() const = 0;
};

/* Takes a spritesheet and deduces frame to show
 *
 * hcount and vcount are, respectively, the count of horizontal
 * and vertical frames on the spritesheet. The x,y position given
 * is relative to the 0,0 position of the screen, not the map,
 * and is not isometric.
 */
class GridAnimation : public Animation {
    SDL_Renderer *renderer;
    SDL_Rect clip;
    Texture spritesheet;
    unsigned hcount, vcount;

    public:
    GridAnimation(SDL_Renderer *renderer, const std::string &filename,
            unsigned hcount, unsigned vcount);

    void renderFrame(int frame, int x, int y);

    int getHeight() const;
    int getWidth() const;
};

/* Ignores the frame argument and shows a static texture */
class StaticFrame : public Animation {
    SDL_Renderer *renderer;
    Texture sprite;

    public:
    StaticFrame(SDL_Renderer *renderer, const std::string &filename);

    void renderFrame(int frame, int x, int y);

    int getHeight() const;
    int getWidth() const;
};

#endif
