#include "Animation.h"
#include <SDL2/SDL.h>
#include "Texture.h"
#include <stdexcept>

GridAnimation::GridAnimation(SDL_Renderer *renderer,
        const std::string &filename, int hcount, int vcount, int padding)
        : renderer(renderer), clip(), spritesheet(),
        hcount(hcount), vcount(vcount), padding(padding) {
    if (! renderer) throw std::runtime_error("null renderer");

    spritesheet.loadFromFile(filename, renderer);

    // Dimensions of the clip are deduced from those of the spritesheet
    clip = { 0, 0,
        (spritesheet.getWidth() - padding * (hcount - 1)) / hcount,
        (spritesheet.getHeight() - padding * (vcount - 1)) / vcount};
}

void GridAnimation::renderFrame(int frame, int x, int y) {
    clip.x = frame % hcount * (clip.w + padding);
    clip.y = frame / hcount % vcount * (clip.h + padding);

    spritesheet.renderSprite(renderer, x, y, &clip);
}

int GridAnimation::getHeight() const {
    return clip.h;
}

int GridAnimation::getWidth() const {
    return clip.w;
}
