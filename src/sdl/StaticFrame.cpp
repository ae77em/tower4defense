#include "Animation.h"
#include <SDL2/SDL.h>
#include "Texture.h"
#include <stdexcept>

StaticFrame::StaticFrame(SDL_Renderer *renderer,
        const std::string &filename)
        : renderer(renderer), sprite() {
    if (! renderer) throw std::runtime_error("null renderer");
    sprite.loadFromFile(filename, renderer);
}

void StaticFrame::renderFrame(int frame, int x, int y) {
    sprite.renderSprite(renderer, x, y, nullptr);
}

int StaticFrame::getHeight() const {
    return sprite.getHeight();
}

int StaticFrame::getWidth() const {
    return sprite.getWidth();
}
