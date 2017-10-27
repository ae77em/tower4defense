#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../sdl/LTexture.h"

class Screen {
    SDL_Renderer *renderer;
    SDL_Window *window;
    LTexture tile, tower;

public:
    Screen();
    ~Screen();

    void draw();
    void putTower(unsigned x, unsigned y);
    void putTile(unsigned x, unsigned y);
};

#endif
