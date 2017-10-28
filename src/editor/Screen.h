#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../sdl/LTexture.h"
#include "../sdl/Dot.h"
#include "../sdl/Constants.h"

class Screen {
    SDL_Renderer *renderer;
    SDL_Window *window;
    LTexture tile, tower;
    Dot dot;
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

public:
    Screen();
    ~Screen();

    void draw();
    void putTower(unsigned x, unsigned y);
    void putTile(unsigned x, unsigned y);
    void handleEvent(SDL_Event &e);
    void clear();
};

#endif
