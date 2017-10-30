#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../sdl/LTexture.h"
#include "../sdl/Dot.h"
#include "../sdl/Constants.h"
#include "../common/modelo/Mapa.h"
#include "../common/Point.h"

class Screen {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Dot dot;
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    LTexture tile, waterTower, earthTower, fireTower, airTower;

    void putWaterTower(unsigned x, unsigned y);
    void putEarthTower(unsigned x, unsigned y);
    void putFireTower(unsigned x, unsigned y);
    void putAirTower(unsigned x, unsigned y);
    void putTile(unsigned x, unsigned y);

public:
    Screen();
    ~Screen();

    void draw();
    void put(Mapa &map);
    void handleEvent(SDL_Event &e);
    void clear();
    Point mouseCurrentTile();
};

#endif
