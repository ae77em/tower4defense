#include "Animation.h"
#include <SDL2/SDL.h>
#include "Texture.h"
#include <stdexcept>

GridAnimation::GridAnimation(SDL_Renderer *renderer,
        const std::string &filename, unsigned hcount, unsigned vcount)
        : renderer(renderer), clip(),
        spritesheet(), hcount(hcount), vcount(vcount) {
    if (! renderer) throw std::runtime_error("null renderer");

    spritesheet.loadFromFile(filename, renderer);

    // Dimensions of the clip are deduced from those of the spritesheet
    clip = { 0, 0,
        (int)( spritesheet.getWidth() / hcount ),
        (int)( spritesheet.getHeight() / vcount )};
}

void GridAnimation::renderFrame(int frame, int x, int y) {
    clip.x = frame % hcount * clip.w;
    clip.y = frame / hcount % vcount * clip.h;

    spritesheet.renderSprite(renderer, x, y, &clip);
}

int GridAnimation::getHeight() const {
    return clip.h;
}

int GridAnimation::getWidth() const {
    return clip.w;
}
