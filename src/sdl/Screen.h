#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.h"
#include "Animation.h"
#include "Dot.h"
#include "Constants.h"
#include "../common/modelo/Mapa.h"
#include "../common/Point.h"

class Screen {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Dot dot;
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    TTF_Font *font;
    LTexture dialog;
    LTexture tile, waterTower, earthTower, fireTower, airTower;
    Animation *portal;

    void putDialog();

    void put(unsigned x, unsigned y, LTexture &texture);

public:
    Screen();
    ~Screen();

    void clear();
    void put(Mapa &map);
    void trace(const std::vector<Point> &path);
    void setDialog(const std::string &text);
    void draw();

    void handleEvent(SDL_Event &e);
    Point mouseCurrentTile();
};

#endif
