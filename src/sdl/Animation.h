#include <SDL2/SDL.h>
#include "Texture.h"
#include "../common/Point.h"
#include <utility>
#include <memory>
#include <vector>

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
    int hcount, vcount;
    int padding;

    public:
    /* Create a new animation from the given sprite sheet

       hcount is the number of frames in the horizontal
       vcount is the number of frames in the vertical
       padding is the number of pixels between any two adjacent frames
     */
    GridAnimation(SDL_Renderer *renderer, const std::string &filename,
            int hcount, int vcount, int padding = 0);

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

typedef std::pair<std::unique_ptr<Animation>, Point> Record;
class CompositeAnimation : public Animation {
    std::vector<Record> members;

    public:
    CompositeAnimation();

    /* Adds a member to the top of the composite

       The new member will be displayed last, at the given relative
       position.
     */
    void memberPushBack(Animation *m, int x = 0, int y = 0);

    /*  -+---> x       ^
         |             |
       y v  +---+      | h
            |   |---@  |
            B---+   |  |
              |     |  |
              A-----@  v

       Assuming a composite with elements as shown, the height h
       of the sprite is:

           h = max(y_offset + elem_height)

       Similar considerations apply to the width.
     */
    void renderFrame(int frame, int x, int y);

    int getHeight() const;
    int getWidth() const;
};

#endif
